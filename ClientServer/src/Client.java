import java.io.*;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Client {
	public static void main(String args[]) throws UnknownHostException, IOException
	{
		Scanner sc = new Scanner(System.in);
		Socket s = new Socket("127.0.0.1",1324);
		Scanner sc1 = new Scanner(s.getInputStream());
		System.out.println("Enter the file name: ");
		String fileName = sc.nextLine();
		//File filePointer = new File(fileName);
		FileInputStream fileIn = new FileInputStream(fileName);
		DataInputStream dataIn = new DataInputStream(fileIn);
		String k;
		while((k=dataIn.readUTF())!=null) 
		{
			//byte b[] = new byte[100];
			System.out.println(k+"");
			//System.out.println(dataIn.read(b));
		}
		//int number = sc.nextInt();
		//Pass the number to the server
		//PrintStream p = new PrintStream(s.getOutputStream());
		//p.println(number);
		//int temp = sc1.nextInt();
		//System.out.println(temp);
	}
}
