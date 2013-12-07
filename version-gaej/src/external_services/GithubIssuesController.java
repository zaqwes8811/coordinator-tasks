package external_services;

import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Label;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.client.PageIterator;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;

import java.io.IOException;
import java.util.*;

// About:
//   Работает с issues конкретного, в данном случае, Github-багтрекера.
public class GithubIssuesController {
  private final GitHubClient CLIENT_;
  private final String REPO_NAME_;
  GithubIssuesController(GitHubClient client, String repoName) {
    CLIENT_ = client;
    REPO_NAME_ = repoName;
  }

  public List<String> getTitlesClosedIn(String labelsFilter) throws IOException {
    // Make request filter.
    Map<String, String> filter = new HashMap<String, String>();
    filter.put(IssueService.FILTER_LABELS, labelsFilter);
    filter.put(IssueService.FILTER_STATE, IssueService.STATE_CLOSED);

    // Real work with service.
    RepositoryService service = new RepositoryService(CLIENT_);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);

    IssueService issueService = new IssueService(CLIENT_);
    PageIterator<Issue> pageIssues = issueService.pageIssues(repo, filter);

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

  public List<Issue> getAllIssues() throws IOException {
    // Real work with service.
    RepositoryService service = new RepositoryService(CLIENT_);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);

    IssueService issueService = new IssueService(CLIENT_);
    PageIterator<Issue> issuesIterator = issueService.pageIssues(repo);

    // За раз читаем всю страницу
    List<Issue> issues = new ArrayList<Issue>();
    while(issuesIterator.hasNext()) {
      Collection<Issue> page = issuesIterator.next();
      issues.addAll(page);
    }
    return issues;
  }
}
