/*Determine whether a string is a comment */
import java.util.regex.Pattern;
import java.util.Scanner;

public class Comments {
  public static void main(String[] args) {

  
  String comment = "";

  Scanner keyboard = new Scanner(System.in);
  System.out.println("Enter string to check if valid comment");
  comment = keyboard.nextLine();
    
/* boolean PatternMatch = Pattern.matches("([a-zA-Z0-9]+|[#!%$‘&+*–/= ?^_`{|}~])+.([a-zA-Z0-9]|[#!%$‘&+*–/= ?^_`{|}~])+", Email); */

boolean PatternMatch = 
Pattern.matches("\\/\\*(.*?)\\*\\/", comment);

    
    if(PatternMatch) {
      System.out.println("Valid comment ");
    } else {
      System.out.println("Invalid comment");
  }
}
}
