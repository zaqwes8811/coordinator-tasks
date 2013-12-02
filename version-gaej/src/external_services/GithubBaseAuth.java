package external_services;

import org.checkthread.annotations.NotThreadSafe;
import org.eclipse.egit.github.core.client.GitHubClient;

@NotThreadSafe
public class GithubBaseAuth {
  static public final  String USER_NAME = "zaqwes8811";
  private final  String ASDF = "a3fs4gd5h";

  public GitHubClient createBasicAuthClient() {
    // Basic authentication
    GitHubClient client = new GitHubClient();
    client.setCredentials(USER_NAME, ASDF);
    return client;
  }
}
