package org.hermesinstaller.scenes;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.Separator;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.stage.DirectoryChooser;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class PostgreSQL extends Scene {

    private Label postgreSQLInstallation;
    private Button chooseInstallation;
    private Button install;
    private TextField port;
    private PasswordField password;
    private HBox portHBox;
    private HBox passwordHBox;

    public PostgreSQL(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Choose PostgreSQL Installation"), 0, 2);

        //4th row
        postgreSQLInstallation = new Label("Please choose a PostgreSQL installation.");
        postgreSQLInstallation.setWrapText(true);

        grid.add(postgreSQLInstallation, 0, 3);

        //5th row
        chooseInstallation = new Button("Choose an installation");
        chooseInstallation.setOnAction((ActionEvent e) -> {
            DirectoryChooser directoryChooser = new DirectoryChooser();
            File file = directoryChooser.showDialog(InstallerEnvironment.primaryStage);
            if (file != null) {
                String string = file.getAbsolutePath();
                try {
                    Process process = Runtime.getRuntime().exec(new String[]{string + File.separatorChar + "bin" + File.separatorChar + "pg_config", "--version"});
                    process.waitFor();
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                    String version = bufferedReader.readLine();

                    String arch;
                    if (InstallerEnvironment.osName.get().contains("Windows")) {
                        process = Runtime.getRuntime().exec(new String[]{"File\\file", string + File.separatorChar + "bin" + File.separatorChar + "pg_config.exe"});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        bufferedReader.readLine();
                        arch = bufferedReader.readLine();

                        if (arch.contains("64bit")) {
                            arch = "64bit";
                        } else if (arch.contains("32bit")) {
                            arch = "32bit";
                        }
                    } else if (InstallerEnvironment.osName.get().contains("Mac OS X")) {
                        process = Runtime.getRuntime().exec(new String[]{"file", string + File.separatorChar + "bin" + File.separatorChar + "pg_config"});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));

                        boolean bit64 = false, bit32 = false;
                        while ((arch = bufferedReader.readLine()) != null) {
                            if (arch.contains("x86_64")) {
                                bit64 = true;
                            } else if (arch.contains("i386")) {
                                bit32 = true;
                            }
                        }

                        if (bit64) {
                            arch = "64bit";
                        } else if (bit32) {
                            arch = "32bit";
                        }
                    } else {
                        process = Runtime.getRuntime().exec(new String[]{"file", string + File.separatorChar + "bin" + File.separatorChar + "pg_config"});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        arch = bufferedReader.readLine();

                        if (arch.contains("64-bit")) {
                            arch = "64bit";
                        } else if (arch.contains("32-bit")) {
                            arch = "32bit";
                        }
                    }

                    if (version.contains(InstallerEnvironment.postgreSQLVersion.get()) && arch.equals(InstallerEnvironment.postgreSQLArch.get())) {
                        process = Runtime.getRuntime().exec(new String[]{string + File.separatorChar + "bin" + File.separatorChar + "pg_config", "--pkglibdir"});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        InstallerEnvironment.postgreSQLPKGLIBDIRPath.set(bufferedReader.readLine());
                        InstallerEnvironment.postgreSQLInstallationPath.set(string);

                        updatePostgreSQLInstallation();
                    } else {
                        PredefinedUIControls.showMessageDialog("The PostgreSQL installation you chose is not compatible!");
                    }
                } catch (InterruptedException | IOException ex) {
                    PredefinedUIControls.showMessageDialog("The directory you gave is not a PostgreSQL installation!");

                    Logger.getLogger(PostgreSQL.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });

        grid.add(chooseInstallation, 0, 4);

        //6th row
        Separator separator = new Separator();

        grid.add(separator, 0, 5);

        //7th row
        Label portLabel = new Label("PostgreSQL Port:");
        port = new TextField("5432");

        portHBox = new HBox(10);
        portHBox.getChildren().addAll(portLabel, port);

        portHBox.setDisable(true);

        grid.add(portHBox, 0, 6);

        //8th row
        Label passwordLabel = new Label("PostgreSQL Password:");
        password = new PasswordField();

        passwordHBox = new HBox(10);
        passwordHBox.getChildren().addAll(passwordLabel, password);

        passwordHBox.setDisable(true);

        grid.add(passwordHBox, 0, 7);

        //9th row
        Button back = new Button("Back");
        back.setOnAction((ActionEvent e) -> {
            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.configurationScene);
        });

        install = new Button("Install");
        install.setOnAction((ActionEvent e) -> {
            if (!port.getText().matches("^[0-9]+$")) {
                PredefinedUIControls.showMessageDialog("Give a valid port number!");

                return;
            }

            if (password.getText().equals("")) {
                PredefinedUIControls.showMessageDialog("Give a password!");

                return;
            }

            InstallerEnvironment.postgreSQLPort.set(port.getText());
            InstallerEnvironment.postgreSQLPassword.set(password.getText());

            if (InstallerEnvironment.installHermes()) {
                InstallerEnvironment.primaryStage.setScene(new Success(PredefinedUIControls.rootGrid()));
            }
        });
        install.setDisable(true);

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(back, install);

        grid.add(buttonHBox, 1, 8);
    }

    public final void updatePostgreSQLInstallation() {
        postgreSQLInstallation.setText("The folowing PostgreSQL installation was found: " + InstallerEnvironment.postgreSQLInstallationPath.get() + ". Proceed with it?");
        chooseInstallation.setText("Choose a different installation");
        portHBox.setDisable(false);
        passwordHBox.setDisable(false);
        install.setDisable(false);
    }
}
