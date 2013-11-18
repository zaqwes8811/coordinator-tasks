import junit.framework.TestCase;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.GitHubService;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.HashMap;
import java.util.Map;

// http://stackoverflow.com/questions/11326045/github-java-api-search-repos

/**
 * Created with IntelliJ IDEA.
 * User: zaqwes_user
 * Date: 18.11.13
 * Time: 18:58
 * To change this template use File | Settings | File Templates.
 */
public class MainTest extends TestCase{
    private final  String USER_NAME = "zaqwes8811";
    private final  String ASDF = "a3fs4gd5h";

    private GitHubClient createAuthClient() {
        // Basic authentication
        GitHubClient client = new GitHubClient();
        client.setCredentials(USER_NAME, ASDF);
        return client;
    }
    @Test
    public void testMultiply() throws Exception {
        RepositoryService service = new RepositoryService(createAuthClient());
        for (Repository repo : service.getRepositories(USER_NAME)) {
            System.out.println(repo.getName());
            try {
                // Интерфейс не работает
                IssueService issueService = new IssueService();
                System.out.println(issueService.getIssue(repo, 1).getAssignee());
                break;
            } catch (RequestException e) {
                System.out.println("No issues");
            }
        }
    }

    @Test
    public void testIssuesAccess() throws Exception {
        RepositoryService service = new RepositoryService(createAuthClient());
        Map<String, String> filterData = new HashMap<String, String>();
        filterData.put(RepositoryService.FIELD_NAME, "slap");
        for (Repository repo : service.getRepositories(filterData)) {
            System.out.println(repo.getName());
        }
    }
}
