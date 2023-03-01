/*Determine whether a string is a valid email address or not */
import java.util.regex.Pattern;
import java.util.Scanner;

public class Emailaddresscode {
  public static void main(String[] args) {

  
  String Email = "";

  Scanner keyboard = new Scanner(System.in);
  System.out.println("Enter email address to check if valid");
  Email = keyboard.nextLine();
    
//boolean PatternMatch = Pattern.matches("([a-zA-Z0-9]+|[#!%$‘&+*–/= ?^_`{|}~])+.([a-zA-Z0-9]|[#!%$‘&+*–/= ?^_`{|}~])+", Email);

 matches problem 1, 2 and 3
boolean PatternMatch = 
Pattern.matches("([a-zA-Z0-9]+|[#!%$‘&+*–/= ?^_`{|}~])+.([a-zA-Z0-9]|[#!%$‘&+*–/= ?^_`{|}~])+@( [a-zA-Z0-9]+|[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]).([a-zA-Z0-9]+|[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9])", Email);

    
    if(PatternMatch) {
      System.out.println("Valid Email Address ");
    } else {
      System.out.println("Invalid Email Address");
  }
}
}
