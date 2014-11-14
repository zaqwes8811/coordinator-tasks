# coding: utf-8
import com.sun.star.comp.helper.Bootstrap as Bootstrap

import com.sun.star.uno.XComponentContext as XComponentContext
import com.sun.star.uno.XComponentContext as XComponentContext
import com.sun.star.comp.helper.Bootstrap as Bootstrap
import com.sun.star.lang.XMultiComponentFactory as XMultiComponentFactory
import com.sun.star.bridge.UnoUrlResolver as UnoUrlResolver
import com.sun.star.bridge.XUnoUrlResolver as XUnoUrlResolver
import com.sun.star.beans.XPropertySet as XPropertySet
import com.sun.star.uno.UnoRuntime as UnoRuntime

import java.lang.Object as Object

if __name__=='__main__':
    xcomponentcontext = Bootstrap.createInitialComponentContext(None);
    
    # create a connector, so that it can contact the office
    urlResolver = UnoUrlResolver.create(xcomponentcontext);
 
    initialObject = urlResolver.resolve("uno:socket,host=localhost,port=2002;urp;StarOffice.ServiceManager");
    
    xOfficeFactory = XMultiComponentFactory(UnoRuntime.queryInterface(
      XMultiComponentFactory, initialObject))
    # retrieve the component context as property (it is not yet exported from the office)
    # Query for the XPropertySet interface.
    xProperySet = XPropertySet(UnoRuntime.queryInterface(XPropertySet, xOfficeFactory))

    # Get the default context from the office server.
    """Object oDefaultContext = xProperySet.getPropertyValue("DefaultContext");
    
    # Query for the interface XComponentContext.
    XComponentContext xOfficeComponentContext = (XComponentContext) UnoRuntime.queryInterface(
    XComponentContext.class, oDefaultContext);
    
    # now create the desktop service
    # NOTE: use the office component context here!
    Object oDesktop = xOfficeFactory.createInstanceWithContext("com.sun.star.frame.Desktop", xOfficeComponentContext);
    """
    print 'Done'