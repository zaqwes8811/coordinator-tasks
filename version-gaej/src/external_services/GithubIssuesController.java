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
public class GithubIssuesController {
  private final IssueService SERVICE_;
  private final Repository REPOSITORY_;
  private final Cache<String, Integer> COUNT_CACHE_;
  private final Cache<Integer, Issue> ISSUES_CACHE_;

  GithubIssuesController(
      IssueService issueService,
      Repository repo,
      Cache<String, Integer> countCache,
      Cache<Integer, Issue> issuesCache) {
    SERVICE_ = issueService;
    REPOSITORY_ = repo;
    COUNT_CACHE_ = countCache;
    ISSUES_CACHE_ = issuesCache;
  }

  public List<String> getFiltered(Map<String, String> filter)
      throws IOException {
    PageIterator<Issue> pageIssues = SERVICE_.pageIssues(REPOSITORY_, filter);

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

  public List<Issue> getAllDirect() throws IOException {
    PageIterator<Issue> issuesIterator = SERVICE_.pageIssues(REPOSITORY_);

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
        return issues.size();
      }
    });
  }
}
