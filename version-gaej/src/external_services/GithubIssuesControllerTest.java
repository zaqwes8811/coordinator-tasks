package external_services;

import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Label;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.PageIterator;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.*;

public class GithubIssuesControllerTest extends TestCase {
  @Test
  public void testMultiply() throws Exception {
    RepositoryService service = new RepositoryService(new GithubBaseAuth().createBasicAuthClient());
    for (Repository repo : service.getRepositories(GithubBaseAuth.USER_NAME)) {
      try {
        // Интерфейс не работает
        IssueService issueService = new IssueService(new GithubBaseAuth().createBasicAuthClient());
        String title = issueService.getIssue(repo, 1).getTitle();
      } catch (RequestException e) {
        // 404 например
      }
    }
  }

  @Test
  public void testIssuesAccess() throws Exception {
    RepositoryService service = new RepositoryService(new GithubBaseAuth().createBasicAuthClient());
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, "in-the-vicinity-cc");

    Map<String, String> filter = new HashMap<String, String>();
    filter.put(IssueService.FILTER_LABELS, "V8 VM");
    IssueService issueService = new IssueService(new GithubBaseAuth().createBasicAuthClient());
    PageIterator<Issue> pageIssues = issueService.pageIssues(repo, filter);

    // За раз читаем всю страницу
    List<Issue> issues = new ArrayList<Issue>();
    while(pageIssues.hasNext()) {
      Collection<Issue> page = pageIssues.next();
      issues.addAll(page);
    }

    // Все проблемы прочитаны
    for (final Issue issue: issues) {
      Collection<Label> labels = issue.getLabels();
      assertEquals(false, issue.getTitle().equals(null));
      assertEquals(false, labels.equals(null));
    }
  }
}
