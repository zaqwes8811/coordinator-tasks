package hello;

// Integration with GAE
// http://www.mkyong.com/google-app-engine/google-app-engine-spring-mvc-crud-example-with-datastore-low-level-api/
//   но пишут что при деплое ошибки.

import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.SpringApplication;
import org.springframework.context.annotation.ComponentScan;

@ComponentScan
@EnableAutoConfiguration
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}