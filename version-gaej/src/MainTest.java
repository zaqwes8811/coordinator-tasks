import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Label;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.client.PageIterator;
import org.eclipse.egit.github.core.client.RequestException;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.util.*;

// http://stackoverflow.com/questions/11326045/github-java-api-search-repos


public class MainTest extends TestCase {
    private final  String USER_NAME = "zaqwes8811";
    private final  String ASDF = "a3fs4gd5h";

    static void print(Object arg) {
        System.out.println(arg);
    }

    private GitHubClient createBasicAuthClient() {
        // Basic authentication
        GitHubClient client = new GitHubClient();
        client.setCredentials(USER_NAME, ASDF);
        return client;
    }
    @Test
    public void testMultiply() throws Exception {
        RepositoryService service = new RepositoryService(createBasicAuthClient());
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
        RepositoryService service = new RepositoryService(createBasicAuthClient());
        Repository repo = service.getRepository(USER_NAME, "in-the-vicinity-cc");
        System.out.println(repo.getName());

        Map<String, String> filter = new HashMap<String, String>();
        filter.put(IssueService.FILTER_LABELS, "V8 VM");
        IssueService issueService = new IssueService(createBasicAuthClient());
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
            print(issue.getTitle());
            print("  "+labels);
        }
    }

    @Test
    public void testCreateIssue() throws Exception {

    }

    @Test
    public void testOAth20Github() throws Exception {

    }
    // Failed
    //Map<String, String> filterData = new HashMap<String, String>();
    //filterData.put(RepositoryService.FIELD_NAME, "slap");
}
