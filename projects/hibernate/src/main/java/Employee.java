// FIXME: можно пользоваться и аннотациями - >= java5


import javax.persistence.*;

//
// http://www.coderanch.com/t/487173/ORM/databases/hibernate-sequence-exist
// FIXME: отличия в инкрементации - few notes about a MySQL-to-PostgreSQL migration
//
// http://ivanbabanin.wordpress.com/2010/08/08/hibernate-postgresql/
@Entity
@Table(name = "EMPLOYEE")
public class Employee {
   @Id
   @GeneratedValue  // FIXME: Внимание! могут быть проблемы
   @Column(name = "id")
   private int id;

   @Column(name = "first_name")
   private String firstName;

   @Column(name = "last_name")
   private String lastName;

   @Column(name = "salary")  // можно и без, тогда просто по имени
   private int salary;

   @Transient
   private String nonStored;

   public Employee() {}
   public Employee(String fname, String lname, int salary) {
      this.firstName = fname;
      this.lastName = lname;
      this.salary = salary;
   }
   public int getId() {
      return id;
   }
   public void setId( int id ) {
      this.id = id;
   }
   public String getFirstName() {
      return firstName;
   }
   public void setFirstName( String first_name ) {
      this.firstName = first_name;
   }
   public String getLastName() {
      return lastName;
   }
   public void setLastName( String last_name ) {
      this.lastName = last_name;
   }
   public int getSalary() {
      return salary;
   }
   public void setSalary( int salary ) {
      this.salary = salary;
   }
}