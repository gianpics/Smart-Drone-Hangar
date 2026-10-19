package esiot.drone_remote_unit;

/**
 * 
 * Dashboard for Drone Remote Unit. Assignment
 * 
 * Partially implemented.
 * 
 */
class DashboardLauncher   {

	static DashboardView view = null;
	static LogView log = null;

	public static void main(String[] args) throws Exception {	
		/*
		if (args.length != 1){
			System.err.println("Args: <serial port>");
			System.exit(1);
		}*/

		view = new DashboardView();
		log = new LogView();
		
		String portName = "COM11";
		DashboardController contr = new DashboardController(portName,view,log);
		view.registerController(contr);
		
		log.display();
		view.setLocationRelativeTo(null); 
		view.display();

	}
}