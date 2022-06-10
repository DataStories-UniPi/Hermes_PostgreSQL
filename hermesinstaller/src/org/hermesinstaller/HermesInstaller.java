package org.hermesinstaller;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import org.hermesinstaller.scenes.Configuration;
import org.hermesinstaller.scenes.License;
import org.hermesinstaller.scenes.Operation;
import org.hermesinstaller.scenes.PostgreSQL;
import org.hermesinstaller.scenes.Unsupported;
import org.hermesinstaller.scenes.Update;
import org.hermesinstaller.scenes.Version;

public class HermesInstaller extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        InstallerEnvironment.osName.set(System.getProperty("os.name"));
        InstallerEnvironment.osArch.set(System.getProperty("os.arch"));
        InstallerEnvironment.osUser.set(System.getProperty("user.name"));

        boolean osSupported = true;
        if (InstallerEnvironment.osName.get().contains("Windows")) {
            try {
                //Process process = Runtime.getRuntime().exec("wmic OS get OSArchitecture");
                Process process = Runtime.getRuntime().exec("reg query \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /v PROCESSOR_ARCHITECTURE");
                process.waitFor();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String string;
                while ((string = bufferedReader.readLine()) != null) {
                    if (string.contains("32-bit") || string.contains("x86")) {
                        InstallerEnvironment.osArch.set("x86");
                    } else if (string.contains("64-bit") || string.contains("AMD64")) {
                        InstallerEnvironment.osArch.set("amd64");
                    } else {
                        //osSupported = false;
                    }
                }
            } catch (InterruptedException | IOException ex) {
                Logger.getLogger(HermesInstaller.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else if (InstallerEnvironment.osName.get().contains("Linux") || InstallerEnvironment.osName.get().contains("Mac OS X")) {
            try {
                Process process = Runtime.getRuntime().exec("uname -m");
                process.waitFor();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String string = bufferedReader.readLine();

                if (string.contains("i686") || string.contains("i386")) {
                    InstallerEnvironment.osArch.set("x86");
                } else if (string.contains("x86_64")) {
                    InstallerEnvironment.osArch.set("amd64");
                } else {
                    //osSupported = false;
                }
            } catch (InterruptedException | IOException ex) {
                Logger.getLogger(HermesInstaller.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {
            osSupported = false;
        }

        primaryStage.setTitle("Hermes Installer");
        primaryStage.getIcons().add(new Image(getClass().getResourceAsStream("2000px-Talaria.svg.png")));
        //primaryStage.setFullScreen(true);

        InstallerEnvironment.primaryStage = primaryStage;

        if (osSupported) {
            InstallerEnvironment.operationScene = new Operation(PredefinedUIControls.rootGrid());
            InstallerEnvironment.versionScene = new Version(PredefinedUIControls.rootGrid());
            InstallerEnvironment.licenseScene = new License(PredefinedUIControls.rootGrid());
            InstallerEnvironment.configurationScene = new Configuration(PredefinedUIControls.rootGrid());
            InstallerEnvironment.postgreSQLScene = new PostgreSQL(PredefinedUIControls.rootGrid());
            InstallerEnvironment.updateScene = new Update(PredefinedUIControls.rootGrid());

            primaryStage.setScene(InstallerEnvironment.operationScene);
        } else {
            primaryStage.setScene(new Unsupported(PredefinedUIControls.rootGrid()));
        }

        primaryStage.show();
    }
}
