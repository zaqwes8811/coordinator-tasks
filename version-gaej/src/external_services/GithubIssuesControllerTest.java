package external_services;

import junit.framework.TestCase;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.List;

public class GithubIssuesControllerTest extends TestCase {
  //@DummyTest
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
      break;
    }
  }

  //@DummyTest
  @Test
  public void testGetTitlesClosedIn() throws Exception {
    String repoName = "in-the-vicinity-cc";
    GithubIssuesController controller =
        new GithubIssuesController(
            new GithubBaseAuth().createBasicAuthClient(), "V8 VM");

    List<String> titles = controller.getTitlesClosedIn(repoName);
    assertEquals(false, titles.isEmpty());
  }
}
