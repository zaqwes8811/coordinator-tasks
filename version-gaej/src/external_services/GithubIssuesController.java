package external_services;

import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.PageIterator;
import org.eclipse.egit.github.core.service.IssueService;

import java.io.IOException;
import java.util.*;

// About:
//   Работает с issues конкретного, в данном случае, Github-багтрекера.
public class GithubIssuesController {
  private final IssueService SERVICE_;
  private final Repository REPOSITORY_;

  GithubIssuesController(IssueService issueService, Repository repo) {
    SERVICE_ = issueService;
    REPOSITORY_ = repo;
  }

  public List<String> getTitlesOfClosed(String labelsFilter) throws IOException {
    // Make request filter.
    Map<String, String> filter = new HashMap<String, String>();
    filter.put(IssueService.FILTER_LABELS, labelsFilter);
    filter.put(IssueService.FILTER_STATE, IssueService.STATE_CLOSED);


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

  public List<Issue> getAll() throws IOException {
    PageIterator<Issue> issuesIterator = SERVICE_.pageIssues(REPOSITORY_);

    // За раз читаем всю страницу
    List<Issue> issues = new ArrayList<Issue>();
    while(issuesIterator.hasNext()) {
      Collection<Issue> page = issuesIterator.next();
      issues.addAll(page);
    }
    return issues;
  }

  public int getCountNote() {
    return 0;
  }
}
