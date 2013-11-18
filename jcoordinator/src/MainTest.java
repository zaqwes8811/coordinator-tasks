import junit.framework.TestCase;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

// http://stackoverflow.com/questions/11326045/github-java-api-search-repos

/**
 * Created with IntelliJ IDEA.
 * User: zaqwes_user
 * Date: 18.11.13
 * Time: 18:58
 * To change this template use File | Settings | File Templates.
 */
public class MainTest extends TestCase{
    private final  String userName = "zaqwes8811";
    @Test
    public void testMultiply() throws Exception {
        RepositoryService service = new RepositoryService();
        for (Repository repo : service.getRepositories(userName)) {
            System.out.println(repo.getName());
            try {
                System.out.println(new IssueService().getIssue(repo, 1).getAssignee());
            } catch (RequestException e) {
                System.out.println("No issues");
            }
        }
    }

    @Test
    public void testIssuesAccess() throws Exception {
        // Так а куда передать чей репозиторий?
        //IssueService issueService = new IssueService();
        //issueService.getIssues();
    }
}
