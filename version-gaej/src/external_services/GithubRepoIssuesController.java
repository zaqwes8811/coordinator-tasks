package external_services;

import com.google.common.cache.Cache;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.PageIterator;
import org.eclipse.egit.github.core.service.IssueService;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;

// About:
//   Работает с issues конкретного, в данном случае, Github-багтрекера.
public class GithubRepoIssuesController {
  private final IssueService service_;
  private final Repository repository_;
  private final Cache<String, Integer> COUNT_CACHE_;
  private final Cache<Integer, Issue> issueCache_;

  GithubRepoIssuesController(
      IssueService issueService,
      Repository repo,
      Cache<String, Integer> countCache,
      Cache<Integer, Issue> issuesCache) {
    service_ = issueService;
    repository_ = repo;
    COUNT_CACHE_ = countCache;
    issueCache_ = issuesCache;
  }

  public List<String> getFiltered(Map<String, String> filter)
      throws IOException {
    PageIterator<Issue> pageIssues = service_.pageIssues(repository_, filter);

    // За раз читаем всю страницу
    List<Issue> issues = new ArrayList<Issue>();
    while(pageIssues.hasNext()) {
      Collection<Issue> page = pageIssues.next();
      issues.addAll(page);
    }

    // Все проблемы прочитаны
    List<String> result = new ArrayList<String>();
    for (final Issue issue: issues) {
      result.add(issue.getTitle());
    }
    return result;
  }

  // Constraints:
  //   Опять же это снимок системы.
  //
  // Troubles:
  //   Очень долго даже для 20 задач.
  public List<Issue> getAll() throws ExecutionException {
    Integer countNow = getCountNote();
    List<Issue> issues = new ArrayList<Issue>();
    for (Integer i = 0+1; i < countNow+1; i++) {
      issues.add(getIssue(i));
    }
    return issues;
  }

  // Args:
  //   number > 0
  public Issue getIssue(final Integer number) throws ExecutionException {
    if (number <= 0 || number > getCountNote())
      throw new IllegalArgumentException();

    return issueCache_.get(number, new Callable<Issue>() {
       @Override
       public Issue call() throws Exception {
         return service_.getIssue(repository_, number);
       }
    });
  }

  public List<Issue> getAllDirect() throws IOException {
    PageIterator<Issue> issuesIterator = service_.pageIssues(repository_);

    // За раз читаем всю страницу
    List<Issue> issues = new ArrayList<Issue>();
    while(issuesIterator.hasNext()) {
      Collection<Issue> page = issuesIterator.next();
      issues.addAll(page);
    }
    return issues;
  }

  // Troubles:
  //   При любый операциях возникают проблемы целостности инвариантов.
  //   Если кешировать, то будет проблемы следующего рода.
  //     - в время итерация были добавлены проблемы, или же удалены.
  //
  // Constraints:
  //   Can get only note.
  //
  // Solution:
  //   Значение будет кешироваться, и учитывая его будет рассматриваться замороженная система
  //     на время пока значение не будет вытолкнуто из кеша.
  public Integer getCountNote() throws ExecutionException {
    String key = "count_issues";
    return COUNT_CACHE_.get(key, new Callable<Integer>() {
      @Override
      public Integer call() throws Exception {
        List<Issue> issues = getAllDirect();
        // Похоже придется их вставить в кеш!
        return issues.size();
      }
    });
  }
}
