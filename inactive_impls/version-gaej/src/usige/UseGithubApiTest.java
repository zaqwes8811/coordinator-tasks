package usige;

import external_services.GithubBaseAuth;
import junit.framework.TestCase;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

/**
 * Created with IntelliJ IDEA.
 * User: zaqwes_user
 * Date: 07.12.13
 * Time: 20:16
 * To change this template use File | Settings | File Templates.
 */
public class UseGithubApiTest extends TestCase {

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
}
