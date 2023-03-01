

/**
 **Determine whether value entered is an integer 

 * @author Shawn
 */



import java.util.regex.Pattern;
import java.util.Scanner;

public class IntegerCode {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
         
  String value = "";

  Scanner keyboard = new Scanner(System.in);
  System.out.println("Enter number to check if valid integer");
  value = keyboard.nextLine();
    
/* boolean PatternMatch = Pattern.matches("([a-zA-Z0-9]+|[#!%$‘&+*–/= ?^_`{|}~])+.([a-zA-Z0-9]|[#!%$‘&+*–/= ?^_`{|}~])+", Email); */

boolean PatternMatch = 
Pattern.matches("([0-7]+|[1-9][0-9]*|0[xX][0-9a-fA-F]+)((u|U)?(i64|I64)?(l|L)?)", value);

    
    if(PatternMatch) {
      System.out.println("Valid Integer");
    } else {
      System.out.println("Not a valid integer");
  }
    }
    
}
