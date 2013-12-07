package external_services;

import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.List;

public class GithubIssuesControllerTest extends TestCase {
  private final String REPO_NAME_ = "coordinator-tasks";

  @Test
  public void testGetTitlesClosedIn() throws Exception {
    GithubIssuesController controller =
        new GithubIssuesController(
            new GithubBaseAuth().createBasicAuthClient(), REPO_NAME_);

    List<String> titles = controller.getTitlesClosedIn("V8 VM");
    assertTrue(titles.isEmpty());
  }

  @Test
  public void testGetAllIssues() throws Exception {
    GithubBaseAuth client = new GithubBaseAuth();
    GithubIssuesController controller = new GithubIssuesController(
       client.createBasicAuthClient(), REPO_NAME_);

    List<Issue> issues = controller.getAllIssues();
    assertFalse(issues.isEmpty());
  }
}
