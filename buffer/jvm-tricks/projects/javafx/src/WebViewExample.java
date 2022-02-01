import com.google.common.collect.ImmutableMap;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Accordion;
import javafx.scene.control.TitledPane;
import javafx.scene.paint.Color;
import javafx.scene.web.WebView;
import javafx.scene.web.WebViewBuilder;
import javafx.stage.Stage;

import java.util.Map;

/**
 * Simple example of using JavaFX 2.0's WebView class.
 *
 * @author Dustin
 */
public class WebViewExample extends Application
{
  /**
   * Provide an instance of a JavaFX 2.0 Accordion control with each titled
   * pane in the Accordion featuring a title based on a 'key' value in the
   * provided a map and including WebView content of the page referenced by the
   * URL in the 'value' portion of the map.
   *
   * @param titleToUrl Mapping of page titles to their URLs to be used as
   *    titled pane titles and source of content respectively.
   * @return Accordion control with web page-based titled panes.
   */
  private Accordion prepareAccordion(final Map<String, String> titleToUrl)
  {
    final Accordion accordion = new Accordion();
    for (final Map.Entry<String,String> webMap : titleToUrl.entrySet())
    {
      final TitledPane pane =
        new TitledPane(webMap.getKey(), buildWebView(webMap.getValue()));
      accordion.getPanes().add(pane);
    }
    return accordion;
  }

  /**
   * Build a simple WebView based on the provided URL.
   *
   * @param url URL from which content will be rendered in the provided WebView.
   * @return WebView whose content is based on web page at provided URL.
   */
  private WebView buildWebView(final String url)
  {
    final WebView webView =
      WebViewBuilder.create().prefHeight(450).prefWidth(1000).build();
    webView.getEngine().load(url);
    return webView;
  }

  /**
   * JavaFX 2.0's Application.start(Stage) method.
   *
   * @param stage Primary stage.
   * @throws Exception Exception thrown during execution of JavaFX application
   *    stage.
   */
  @Override
  public void start(final Stage stage) throws Exception
  {
    stage.setTitle("JavaFX 2.0 WebView Example: Favorite Java Web Sites");
    final Group rootGroup = new Group();
    final Map<String,String> titleToUrl =
      ImmutableMap.<String,String>builder()
        .put("Inspired by Actual Events", "http://marxsoftware.blogspot.com/")
        .put("JavaWorld", "http://javaworld.com/")
        .put("Java.net", "http://java.net/")
        .put("Java Lobby", "http://google.com/")
        .build();
    rootGroup.getChildren().add(prepareAccordion(titleToUrl));
    final Scene scene = new Scene(rootGroup, 1000, 600, Color.WHITE);
    stage.setScene(scene);
    stage.show();
  }

  /**
   * Main function for running this JavaFX example.
   *
   * @param arguments Command-line arguments: none expected.
   */
  public static void main(final String[] arguments)
  {
    Application.launch(arguments);
  }
}
