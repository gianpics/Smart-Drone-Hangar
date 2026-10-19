package esiot.drone_remote_unit;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.*;

class DashboardView extends JFrame implements ActionListener {

	private JLabel droneStateValue;
	private JLabel hangarStateValue;
	private JLabel distanceValue;

	private JButton takeOffButton;
	private JButton landingButton;

	private DashboardController controller;

	public DashboardView() {
		super(".:: Drone Remote Unit ::.");
		setSize(300, 200);
		this.setResizable(false);

		boolean startingState = false;

		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 15)));

		// Drone State
		mainPanel.add(buildInfoRow("Drone State:", droneStateValue = new JLabel("--")));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 8)));

		// Hangar State
		mainPanel.add(buildInfoRow("Hangar State:", hangarStateValue = new JLabel("--")));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 8)));

		// Distance
		mainPanel.add(buildInfoRow("Distance (m):", distanceValue = new JLabel("--")));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 15)));

		// Command buttons
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
		buttonPanel.add(Box.createRigidArea(new Dimension(30, 0)));

		takeOffButton = new JButton("TAKE OFF");
		takeOffButton.setEnabled(startingState);
		takeOffButton.addActionListener(this);
		buttonPanel.add(takeOffButton);

		buttonPanel.add(Box.createRigidArea(new Dimension(20, 0)));

		landingButton = new JButton("LANDING");
		landingButton.setEnabled(startingState);
		landingButton.addActionListener(this);
		buttonPanel.add(landingButton);

		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0, 15)));

		setContentPane(mainPanel);

		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				System.exit(-1);
			}
		});
	}

	// Helper
	private JPanel buildInfoRow(String labelText, JLabel valueLabel) {
		JPanel row = new JPanel();
		row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
		row.add(Box.createRigidArea(new Dimension(30, 0)));
		JLabel label = new JLabel(labelText);
		label.setPreferredSize(new Dimension(130, 20));
		label.setMinimumSize(new Dimension(130, 20));
		label.setMaximumSize(new Dimension(130, 20));
		row.add(label);
		row.add(valueLabel);
		return row;
	}

	public void display() {
		SwingUtilities.invokeLater(() -> {
			this.setVisible(true);
		});
	}

	public void registerController(DashboardController contr) {
		this.controller = contr;
	}


	// Update dasboard controls
	public void updateControls(String droneState, String hangarState, String distance) {
		SwingUtilities.invokeLater(() -> {
			setDroneState(droneState);
			setHangarState(hangarState);
			setDistance(distance);

			boolean alarm = "ALARM".equals(hangarState);
			takeOffButton.setEnabled("REST".equals(droneState) && !alarm);
			landingButton.setEnabled("OPERATING".equals(droneState) && !alarm);
		});
	}

	// Label update (called by MonitoringAgent)
	public void setDroneState(String state) {
		SwingUtilities.invokeLater(() -> {
			droneStateValue.setText(state);
		});
	}

	// Label update
	public void setHangarState(String state) {
		SwingUtilities.invokeLater(() -> {
			hangarStateValue.setForeground(state.equals("ALARM") ? Color.RED : Color.BLACK);	
			hangarStateValue.setText(state);
		});
	}

	// Label update (can be "--")
	public void setDistance(String dist) {
		SwingUtilities.invokeLater(() -> {
			distanceValue.setText(dist);
		});
	}

	// Button click handler
	public void actionPerformed(ActionEvent ev) {
		if (controller == null) return;
		if (ev.getSource() == takeOffButton) {
			controller.sendTakeOff();
		} else if (ev.getSource() == landingButton) {
			controller.sendLanding();
		}
	}
}
