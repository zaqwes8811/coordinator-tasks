import org.apache.oltu.oauth2.client.OAuthClient;
import org.apache.oltu.oauth2.client.URLConnectionClient;
import org.apache.oltu.oauth2.client.request.OAuthClientRequest;
import org.apache.oltu.oauth2.client.response.GitHubTokenResponse;
import org.apache.oltu.oauth2.common.OAuthProviderType;
import org.apache.oltu.oauth2.common.exception.OAuthProblemException;
import org.apache.oltu.oauth2.common.message.types.GrantType;
import org.eclipse.egit.github.core.client.GitHubClient;

import java.io.BufferedReader;
import java.io.InputStreamReader;

// http://www.vogella.com/articles/JUnit/article.html
public class Main {

    public static void main(String[] args) throws Exception {
        try {
            OAuthClientRequest request = OAuthClientRequest
                    .authorizationProvider(OAuthProviderType.GITHUB)
                    .setClientId("e403e7f2055d3830be63")
                    //.setRedirectURI("http://localhost:8080/")
                    .buildQueryMessage();

            //in web application you make redirection to uri:
            System.out.println("Visit: " + request.getLocationUri() + "\nand grant permission");


            // Нормально думаю можно сделать только на GAE, своего url пока нету
            System.out.print("Now enter the OAuth code you have received in redirect uri:");

            // Нужно плучить реальный ответ, для чего у системы должен быть рельный url
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            String code = br.readLine();

            /// Exchange OAuth code for an access token
            request = OAuthClientRequest
                    .tokenProvider(OAuthProviderType.GITHUB)
                    .setGrantType(GrantType.AUTHORIZATION_CODE)
                    .setClientId("e403e7f2055d3830be63")
                    .setClientSecret("6b3094412afd21c4a53d31fa5a628de754ee809c")
                    .setRedirectURI("http://localhost:8080/")
                    .setCode(code)
                    .buildBodyMessage();

            OAuthClient oAuthClient = new OAuthClient(new URLConnectionClient());

            //Facebook is not fully compatible with OAuth 2.0 draft 10, access token response is
            //application/x-www-form-urlencoded, not json encoded so we use dedicated response class for that
            //Own response class is an easy way to deal with oauth providers that introduce modifications to
            //OAuth specification
            GitHubTokenResponse oAuthResponse = oAuthClient.accessToken(request, GitHubTokenResponse.class);

            System.out.println(
                    "Access Token: " + oAuthResponse.getAccessToken() + ", Expires in: " + oAuthResponse
                            .getExpiresIn());
            String token = oAuthResponse.getAccessToken();
            // Делаем запрос к Issues
            GitHubClient client = new GitHubClient();
            client.setOAuth2Token(token);
        } catch (OAuthProblemException e) {
            System.out.println("OAuth error: " + e.getError());
            System.out.println("OAuth error description: " + e.getDescription());
        }
    }
}
