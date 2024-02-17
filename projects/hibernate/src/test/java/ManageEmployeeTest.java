//package sandbox;
import java.util.Iterator;
import java.util.List;

import org.hibernate.cfg.AnnotationConfiguration;
import org.junit.After;
import org.junit.Test;

import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;

public class ManageEmployeeTest {
   private static SessionFactory factory;

   @After
   public void tearDown() {
      // FIXME: remove all created
   }

   // FIXME: поробовать отношения и запросы посложнее select
   @Test
   public void testRelations() {
      // одна из строно управляет зависимостью

   }

   @Test
   public void testCriteriaQueries() {

   }

   @Test
   public void testBatchProcessing() {

   }

   // FIXME: если ничего не настроено исключений не выозникает!!
   // Тест не запустился - неправильное было имя файла
   @Test
   public void testInitAndCRUD() {
      System.out.println("Working Directory = " + System.getProperty("user.dir"));
      try{
         // FIXME: http://stackoverflow.com/questions/8621906/is-buildsessionfactory-deprecated-in-hibernate-4
         // http://stackoverflow.com/questions/17911308/looking-for-a-not-deprecated-session-factory
         //
         // "/resources/hibernate.cfg.xml"
         // http://stackoverflow.com/questions/18736594/location-of-hibernate-cfg-xml-in-project
         //
         // With annotation
         //   http://www.tutorialspoint.com/hibernate/hibernate_annotations.htm

         // Step 1
         // разбор файлов конф., connection и маппинг
         // hibernate.properties or hibernate.cfg.xml
         Configuration configuration = new Configuration();
         configuration.configure();
         configuration.addAnnotatedClass(Employee.class);

         // Step 2
         factory = configuration.buildSessionFactory();

         factory = new AnnotationConfiguration().configure().buildSessionFactory();

      }catch (Throwable ex) { 
         System.err.println("Failed to create sessionFactory object." + ex);
         throw new ExceptionInInitializerError(ex); 
      }
      ManageEmployeeTest ME = new ManageEmployeeTest();

      /* Add few employee records in database */
      Integer empID1 = ME.addEmployee("Zara", "Ali", 1000);
      Integer empID2 = ME.addEmployee("Daisy", "Das", 5000);
      Integer empID3 = ME.addEmployee("John", "Paul", 10000);

      /* List down all the employees */
      ME.listEmployees();

      /* Update employee's records */
      ME.updateEmployee(empID1, 5000);

      /* Delete an employee from the database */
      ME.deleteEmployee(empID2);

      /* List down new list of the employees */
      ME.listEmployees();
   }
   /* Method to CREATE an employee in the database */
   public Integer addEmployee(String fname, String lname, int salary){
      // lightweight and non thread-safe. disposable
      // FIXME: а как пул соединений? Похоже можно задачть что-то через конфигурацию
      Session session = factory.openSession();

      Transaction tx = null;
      Integer employeeID = null;
      try{
         tx = session.beginTransaction();

         // transient value
         Employee employee = new Employee(fname, lname, salary);

         employeeID = (Integer) session.save(employee);

         tx.commit();

         // employee - persistent value
      } catch (HibernateException e) {
         if (tx != null)
            tx.rollback();

         e.printStackTrace();
      }finally {
         session.close(); 
      }
      return employeeID;
   }
   /* Method to  READ all the employees */
   public void listEmployees( ){
      Session session = factory.openSession();
      Transaction tx = null;
      try{
         tx = session.beginTransaction();
         List employees = session.createQuery("FROM Employee").list(); 
         for (Iterator iterator = employees.iterator(); iterator.hasNext();){
            Employee employee = (Employee) iterator.next(); 
            System.out.print("First Name: " + employee.getFirstName()); 
            System.out.print("  Last Name: " + employee.getLastName()); 
            System.out.println("  Salary: " + employee.getSalary()); 
         }
         tx.commit();
      }catch (HibernateException e) {
         if (tx!=null) tx.rollback();
         e.printStackTrace(); 
      }finally {
         session.close(); 
      }
   }
   /* Method to UPDATE salary for an employee */
   public void updateEmployee(Integer EmployeeID, int salary ){
      Session session = factory.openSession();
      Transaction tx = null;
      try{
         tx = session.beginTransaction();
         Employee employee = (Employee)session.get(Employee.class, EmployeeID);
         employee.setSalary( salary );
		 session.update(employee); 
         tx.commit();
      }catch (HibernateException e) {
         if (tx!=null) tx.rollback();
         e.printStackTrace(); 
      }finally {
         session.close(); 
      }
   }
   /* Method to DELETE an employee from the records */
   public void deleteEmployee(Integer EmployeeID){
      Session session = factory.openSession();
      Transaction tx = null;
      try{
         tx = session.beginTransaction();
         Employee employee = 
                   (Employee)session.get(Employee.class, EmployeeID); 
         session.delete(employee); 
         tx.commit();
      }catch (HibernateException e) {
         if (tx!=null) tx.rollback();
         e.printStackTrace(); 
      }finally {
         session.close(); 
      }
   }
}