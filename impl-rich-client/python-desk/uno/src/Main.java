import com.sun.star.connection.ConnectionSetupException;
import com.sun.star.connection.NoConnectException;
import com.sun.star.lang.*;
import com.sun.star.uno.XComponentContext;
import com.sun.star.comp.helper.Bootstrap;
import com.sun.star.bridge.UnoUrlResolver;
import com.sun.star.bridge.XUnoUrlResolver;
import com.sun.star.beans.XPropertySet;
import com.sun.star.uno.UnoRuntime;


public class Main {

    public static void main(String[] args) throws ConnectionSetupException, NoConnectException, com.sun.star.lang.IllegalArgumentException {
      try {
      XComponentContext xcomponentcontext = Bootstrap.createInitialComponentContext(null);

      // create a connector, so that it can contact the office
      XUnoUrlResolver urlResolver = UnoUrlResolver.create(xcomponentcontext);

      Object initialObject = urlResolver.resolve(
        "uno:socket,host=localhost,port=2002;urp;StarOffice.ServiceManager");

      XMultiComponentFactory xOfficeFactory = (XMultiComponentFactory) UnoRuntime.queryInterface(
        XMultiComponentFactory.class, initialObject);

      // retrieve the component context as property (it is not yet exported from the office)
      // Query for the XPropertySet interface.
      XPropertySet xProperySet = (XPropertySet) UnoRuntime.queryInterface(
        XPropertySet.class, xOfficeFactory);

      // Get the default context from the office server.
      Object oDefaultContext = xProperySet.getPropertyValue("DefaultContext");

      // Query for the interface XComponentContext.
      XComponentContext xOfficeComponentContext = (XComponentContext) UnoRuntime.queryInterface(
        XComponentContext.class, oDefaultContext);

      // now create the desktop service
      // NOTE: use the office component context here!
      Object oDesktop = xOfficeFactory.createInstanceWithContext(
        "com.sun.star.frame.Desktop", xOfficeComponentContext);

      // Далее...

      } catch (NoConnectException e) {
        e.printStackTrace();
      } catch (Exception e) {
        e.printStackTrace();
      }
      System.out.println("Done");
    }
}
