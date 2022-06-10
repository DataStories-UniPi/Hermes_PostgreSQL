package org.hermesinstaller.scenes;

import java.io.IOException;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.ParseException;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.Separator;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.Tooltip;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class Version extends Scene {

    public Version(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Choose Hermes Version"), 0, 2);

        //4th row
        Label prerequisitesInfo = new Label("Hermes needs the following prerequisites:\n"
                + "\tPostgreSQL 9.4\n"
                + "\t(Optional) PostGIS 2.1+ (usually installed via Application Stack Builder, available after PostgreSQL installation)\n"
                + "\t(Optional) Python 3.3 (for use with PL/Python)");
        prerequisitesInfo.setWrapText(true);

        grid.add(prerequisitesInfo, 0, 3);

        //5th row
        if (InstallerEnvironment.osName.get().contains("Windows")) {
            ToggleGroup prerequisites = new ToggleGroup();

            RadioButton installedPrerequisites = new RadioButton("I installed them");
            installedPrerequisites.setToggleGroup(prerequisites);

            RadioButton installPrerequisites = new RadioButton("Install prerequisites for me");
            installPrerequisites.setToggleGroup(prerequisites);
            installPrerequisites.setTooltip(new Tooltip("In a next step the installer will launch installation processes for each prerequisite. PostGIS should be installed via StackBuilder."));
            installPrerequisites.selectedProperty().bindBidirectional(InstallerEnvironment.installPrerequisites);

            installedPrerequisites.setSelected(true);

            VBox prerequisitesVBox = new VBox();
            prerequisitesVBox.setSpacing(5);
            prerequisitesVBox.getChildren().addAll(installedPrerequisites, installPrerequisites);

            grid.add(prerequisitesVBox, 0, 4);
        } else {
            Label prerequisitesInfo2 = new Label("If you already installed them you can continue safely.");
            prerequisitesInfo2.setWrapText(true);

            grid.add(prerequisitesInfo2, 0, 4);
        }

        //6th row
        Separator separator = new Separator();

        grid.add(separator, 0, 5);

        //7th row
        Label version = new Label("Choose the version of Hermes you want to install:");
        version.setWrapText(true);

        grid.add(version, 0, 6);

        //8th row
        ToggleGroup versions = new ToggleGroup();

        RadioButton includedVersion = new RadioButton(InstallerEnvironment.includedVersion.get());
        includedVersion.setToggleGroup(versions);
        includedVersion.setTooltip(new Tooltip("This version is the one included in this installer."));

        RadioButton latest = new RadioButton("Latest");
        latest.setToggleGroup(versions);
        latest.setTooltip(new Tooltip("This release is the latest version. An internet connection is required to download it."));
        latest.selectedProperty().bindBidirectional(InstallerEnvironment.isLatest);

        versions.selectToggle(includedVersion);

        try {
            InstallerEnvironment.latestVersion = InstallerEnvironment.URLtoString(InstallerEnvironment.releasesSite + "Latest").replace("\n", "");

            Properties properties = new Properties();
            properties.load(new URL(InstallerEnvironment.releasesSite + InstallerEnvironment.latestVersion + "/installer.properties").openStream());

            String minimumCompatibleInstaller = properties.getProperty("Minimum Compatible Installer");
            String maximumCompatibleInstaller = properties.getProperty("Maximum Compatible Installer");
            if (InstallerEnvironment.isoDateFormat.parse(InstallerEnvironment.installerDate.get()).before(InstallerEnvironment.isoDateFormat.parse(minimumCompatibleInstaller))
                    || InstallerEnvironment.isoDateFormat.parse(InstallerEnvironment.installerDate.get()).after(InstallerEnvironment.isoDateFormat.parse(maximumCompatibleInstaller))) {
                latest.setSelected(false);
                includedVersion.setSelected(true);
                latest.setDisable(true);
                latest.setText("Latest (" + InstallerEnvironment.latestVersion + "). Disabled because this version of the installer is not compatible with the current latest release.\nDownload the latest version of this installer.");
            }

            latest.setText("Latest (" + InstallerEnvironment.latestVersion + ")");
        } catch (IOException | ParseException | NullPointerException ex) {
            latest.setSelected(false);
            includedVersion.setSelected(true);
            latest.setDisable(true);
            latest.setText("Latest. Disabled because it seems there is a problem with your internet connection.");

            Logger.getLogger(Version.class.getName()).log(Level.SEVERE, null, ex);
        }

        VBox versionsVBox = new VBox();
        versionsVBox.setSpacing(5);
        versionsVBox.getChildren().addAll(includedVersion, latest);

        grid.add(versionsVBox, 0, 7);

        //9th row
        Button cancel = new Button("Cancel");
        cancel.setOnAction((ActionEvent e) -> {
            System.exit(0);
            //InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.operationScene);
        });

        Button next = new Button("Next");
        next.setOnAction((ActionEvent e) -> {
            try {
                if (InstallerEnvironment.isLatest.get()) {
                    Properties properties = new Properties();
                    properties.load(new URL(InstallerEnvironment.releasesSite + InstallerEnvironment.latestVersion + "/license.properties").openStream());

                    String string = InstallerEnvironment.URLtoString(InstallerEnvironment.releasesSite + InstallerEnvironment.latestVersion + "/" + properties.getProperty("license"));

                    if (string == null) {
                        PredefinedUIControls.showMessageDialog("Check your internet connection!");

                        return;
                    }

                    InstallerEnvironment.licenseScene.setLicenseText(string);
                } else {
                    Properties properties = new Properties();
                    properties.load(Files.newInputStream(Paths.get("Hermes/license.properties")));

                    InstallerEnvironment.licenseScene.setLicenseText(new String(Files.readAllBytes(Paths.get(properties.getProperty("license"))), Charset.forName("UTF-8")));
                }
            } catch (IOException ex) {
                PredefinedUIControls.showMessageDialog("Unexpected error. Cannot continue!");

                Logger.getLogger(Version.class.getName()).log(Level.SEVERE, null, ex);
                return;
            }

            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.licenseScene);
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(cancel, next);

        grid.add(buttonHBox, 1, 8);
    }
}
