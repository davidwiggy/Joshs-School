// Josh Braegger
// CS1400 

// Prints how many presents were received on each day of the 12 days of 
// Christmas

public class Chapter_3 {
	public static void main(String[] args) {
		int total_presents = 0;
		System.out.println("Day\t\tPresents received\tTotal Presents");
		for (int day = 1; day <= 12; day++) {
			total_presents += day;
			System.out.println(day + "\t\t" + day + "\t\t\t" + total_presents);
		}
	}
}
