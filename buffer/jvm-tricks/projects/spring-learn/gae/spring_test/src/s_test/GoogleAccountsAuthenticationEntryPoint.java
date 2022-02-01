package s_test;

// http://spring.io/blog/2010/08/02/spring-security-in-google-app-engine/
// похоже еще нужно думать о интеграции spring/mvc and spring/secure
// Еще всякие jsp нужны. А для исследовательского приложения нужно разбираться с клиентм дла авторизации
//   OAuth2 что ли

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.users.UserService;
import com.google.appengine.api.users.UserServiceFactory;

public class GoogleAccountsAuthenticationEntryPoint implements AuthenticationEntryPoint {
  public void commence(HttpServletRequest request, HttpServletResponse response, AuthenticationException authException)
      throws IOException, ServletException {
    UserService userService = UserServiceFactory.getUserService();

    response.sendRedirect(userService.createLoginURL(request.getRequestURI()));
  }
}