package external_services;

import com.google.common.cache.Cache;
import com.google.common.cache.CacheBuilder;
import junit.framework.TestCase;
import org.eclipse.egit.github.core.Issue;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.service.IssueService;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;
import org.junit.runners.BlockJUnit4ClassRunner;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;

@RunWith(value = BlockJUnit4ClassRunner.class)
public class GithubRepoIssuesControllerTest extends TestCase {
  private final String REPO_NAME_ = "coordinator-tasks";

  private GithubRepoIssuesController build() throws IOException {
    // Real work with service.
    GitHubClient client =  new GithubBaseAuth().createBasicAuthClient();
    RepositoryService service = new RepositoryService(client);
    Repository repo = service.getRepository(GithubBaseAuth.USER_NAME, REPO_NAME_);
    IssueService issueService = new IssueService(client);

    Cache<String, Integer> countCache = CacheBuilder.newBuilder()
        .maximumSize(1)
        .expireAfterAccess(10, TimeUnit.SECONDS)
        .build();

    Cache<Integer, Issue> issuesCache = CacheBuilder.newBuilder()
        .maximumSize(1000)
        .expireAfterAccess(10, TimeUnit.SECONDS)
        .build();

    GithubRepoIssuesController controller =
        new GithubRepoIssuesController(issueService, repo, countCache, issuesCache);
    return controller;
  }

  @Test
  public void testGetTitlesClosedIn() throws Exception {
    GithubRepoIssuesController controller = build();

    // Make request filter.
    Map<String, String> filter = new HashMap<String, String>();
    filter.put(IssueService.FILTER_LABELS, "V8 VM");
    filter.put(IssueService.FILTER_STATE, IssueService.STATE_CLOSED);
    List<String> titles = controller.getFiltered(filter);
    assertTrue(titles.isEmpty());
  }

  @Test
  public void testGetAllIssues() throws Exception {
    GithubRepoIssuesController controller = build();

    List<Issue> issues = controller.getAllDirect();
    assertFalse(issues.isEmpty());

    // Можно проверить, что индексы все.
  }

  @Test
  public void testGetCountNote() throws Exception {
    GithubRepoIssuesController controller = build();

    Integer count = controller.getCountNote();
    assertFalse(count.equals(0));
    count = controller.getCountNote();
    assertFalse(count.equals(0));
  }

  @Test
  public void testGetIssue() throws Exception {
    GithubRepoIssuesController controller = build();

    Issue count = controller.getIssue(1);
    assertFalse(count.equals(0));
    count = controller.getIssue(1);
    assertFalse(count.equals(0));
  }

  @Test
  public void testGetIssueWithDelay() throws Exception {
    GithubRepoIssuesController controller = build();

    Issue count = controller.getIssue(1);
    assertFalse(count.equals(0));

    //Thread.sleep(2000);
    count = controller.getIssue(1);
    assertFalse(count.equals(0));
  }

  @Test(expected = IllegalArgumentException.class)
  public void testGetIssueZero() throws IOException, ExecutionException {
    GithubRepoIssuesController controller = build();
    Issue count = controller.getIssue(0);
  }

  @Test
  public void testGetAll() throws IOException, ExecutionException {
    GithubRepoIssuesController controller = build();
    controller.getAll();
  }
}
