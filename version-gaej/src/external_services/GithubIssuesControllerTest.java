package external_services;

import com.google.common.cache.Cache;
import com.google.common.cache.CacheBuilder;
import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Test;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class GithubIssuesControllerTest extends TestCase {
  private final String REPO_NAME_ = "coordinator-tasks";

  private GithubIssuesController build() throws IOException {
    // Real work with service.
    GitHubClient client =  new GithubBaseAuth().createBasicAuthClient();
    RepositoryService service = new RepositoryService(client);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);
    IssueService issueService = new IssueService(client);

    Cache<String, Integer> countCache = CacheBuilder.newBuilder()
        .maximumSize(1000)
        .build();

    Cache<Integer, Issue> issuesCache = CacheBuilder.newBuilder()
        .maximumSize(1000)
        .build();

    GithubIssuesController controller =
        new GithubIssuesController(issueService, repo, countCache, issuesCache);
    return controller;
  }

  @Test
  public void testGetTitlesClosedIn() throws Exception {
    GithubIssuesController controller = build();

    // Make request filter.
    Map<String, String> filter = new HashMap<String, String>();
    filter.put(IssueService.FILTER_LABELS, "V8 VM");
    filter.put(IssueService.FILTER_STATE, IssueService.STATE_CLOSED);
    List<String> titles = controller.getFiltered(filter);
    assertTrue(titles.isEmpty());
  }

  @Test
  public void testGetAllIssues() throws Exception {
    GithubIssuesController controller = build();

    List<Issue> issues = controller.getAllDirect();
    assertFalse(issues.isEmpty());

    // Можно проверить, что индексы все.
  }

  @Test
  public void testGetCountNote() throws Exception {
    GithubIssuesController controller = build();

    Integer count = controller.getCountNote();
    assertFalse(count.equals(0));
    count = controller.getCountNote();
    assertFalse(count.equals(0));
  }
}
