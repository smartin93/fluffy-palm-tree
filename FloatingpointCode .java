/*Determine whether a values is a floating point number or not */
import java.util.regex.Pattern;
import java.util.Scanner;

public class FloatingpointCode {
  public static void main(String[] args) {

  
  String floatpt = "";

  Scanner keyboard = new Scanner(System.in);
  System.out.println("Enter floating point number to check if valid");
  floatpt = keyboard.nextLine();
    
/* boolean PatternMatch = Pattern.matches("([a-zA-Z0-9]+|[#!%$‘&+*–/= ?^_`{|}~])+.([a-zA-Z0-9]|[#!%$‘&+*–/= ?^_`{|}~])+", Email); */

boolean PatternMatch = 
Pattern.matches("([0-9]*.[0-9]+|[0-9]+.)([eE][-+]?[0-9]+)?([lLfF]?)|[0-9]+[eE][-+]?[0-9]+[lLfF]?", floatpt);

    
    if(PatternMatch) {
      System.out.println("Valid Floating Point Number ");
    } else {
      System.out.println("Invalid Floating Point Number");
  }
}
}
