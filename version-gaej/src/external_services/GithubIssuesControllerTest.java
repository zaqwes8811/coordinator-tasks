package external_services;

import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.List;

public class GithubIssuesControllerTest extends TestCase {
  private final String REPO_NAME_ = "coordinator-tasks";

  @Test
  public void testGetTitlesClosedIn() throws Exception {
    // Real work with service.
    GitHubClient client =  new GithubBaseAuth().createBasicAuthClient();
    RepositoryService service = new RepositoryService(client);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);
    IssueService issueService = new IssueService(client);

    GithubIssuesController controller = new GithubIssuesController(issueService, repo);

    List<String> titles = controller.getTitlesOfClosed("V8 VM");
    assertTrue(titles.isEmpty());
  }

  @Test
  public void testGetAllIssues() throws Exception {
    // Real work with service.
    GitHubClient client =  new GithubBaseAuth().createBasicAuthClient();
    RepositoryService service = new RepositoryService(client);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);
    IssueService issueService = new IssueService(client);
    GithubIssuesController controller = new GithubIssuesController(issueService, repo);

    List<Issue> issues = controller.getAll();
    assertFalse(issues.isEmpty());
  }
}
