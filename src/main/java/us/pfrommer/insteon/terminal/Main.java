package us.pfrommer.insteon.terminal;

import java.awt.GraphicsEnvironment;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;

import us.pfrommer.insteon.terminal.console.Console;
import us.pfrommer.insteon.terminal.console.gui.FancyGUI;
import us.pfrommer.insteon.terminal.console.gui.GUI;
import us.pfrommer.insteon.terminal.console.terminal.JLineTerminal;
import us.pfrommer.insteon.terminal.console.terminal.Terminal;
import us.pfrommer.insteon.utils.ResourceLocator.ClasspathResourceLocator;

public class Main {
	public static void main(String[] args) {
		Options options = new Options();
		options.addOption("help", false, "Shows help information for the terminal");
		options.addOption("h", false, "Shows help information for the terminal");
		
		options.addOption("gui", false, "Forces the terminal to start in windowed mode");
		options.addOption("g", false, "Same as -gui");
		options.addOption("fancy", false, "If in windowed mode input and output are in the same text area");
		
		options.addOption("nw", false, "Forces the terminal to start in non-windowed mode");
		options.addOption("nojline", false, "If in no-window mode, defaults to a non-jline console implementation");
		
		CommandLineParser parser = new DefaultParser();
		try {
			CommandLine cmd = parser.parse(options, args);
			
			Console c = null;
			
			if (cmd.hasOption("help") || cmd.hasOption("h") || cmd.getArgList().contains("help")) {
				HelpFormatter formatter = new HelpFormatter();
				formatter.printHelp("insteon-terminal", options);
				return;
			}
			
			//Initialize the console correctly
			
			boolean guiMode = !GraphicsEnvironment.isHeadless();
			
			if (cmd.hasOption("nw")) guiMode = false;
			if (cmd.hasOption("gui") || cmd.hasOption("g")) guiMode = true;
			
			if (guiMode) {
				if (cmd.hasOption("fancy")) {
					FancyGUI g = new FancyGUI();
					g.setVisible(true);
					c = g.getConsole();					
				} else {
					c = new GUI();
					((GUI) c).setVisible(true);					
				}
			} else {
				if (cmd.hasOption("nojline")) {
					c = new Terminal();
				} else {
					c = new JLineTerminal();
				}
			}
			
			// Read config from classpath
			Configurator configurator = new Configurator();
			configurator.loadConfig(new ClasspathResourceLocator());
			
			//RUN!
			
			InsteonTerminal interpreter = new InsteonTerminal();
			configurator.configure(interpreter);
			
			interpreter.run(c);
			
		} catch (Exception e) {
			System.err.println("Error: " + e.getMessage());
			System.exit(1);
		}		
	}
}
