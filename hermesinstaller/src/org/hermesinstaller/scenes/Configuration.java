package org.hermesinstaller.scenes;

import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class Configuration extends Scene {

    public Configuration(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Configuration"), 0, 2);

        //4th row
        Label postgreSQLVersionLabel = new Label("What is the version of PostgreSQL that you want Hermes to be installed on?");
        final ChoiceBox postgreSQLVersion = new ChoiceBox(FXCollections.observableArrayList("9.4"));
        postgreSQLVersion.setValue("9.4");

        postgreSQLVersion.valueProperty().addListener((ObservableValue ov, Object t, Object t1) -> {
            //InstallerEnvironment.installPrerequisites.set(true);
        });

        HBox postgreSQLVersionHBox = new HBox(10);
        postgreSQLVersionHBox.getChildren().addAll(postgreSQLVersionLabel, postgreSQLVersion);

        grid.add(postgreSQLVersionHBox, 0, 3);

        //5th row
        final ChoiceBox postgreSQLArch;
        if (InstallerEnvironment.osName.get().contains("Windows")) {
            postgreSQLArch = new ChoiceBox(InstallerEnvironment.osArch.get().equals("x86") ? FXCollections.observableArrayList("32bit") : FXCollections.observableArrayList("32bit", "64bit"));
            postgreSQLArch.setValue(InstallerEnvironment.osArch.get().equals("x86") ? "32bit" : "64bit");

            if (!InstallerEnvironment.osArch.get().equals("x86")) {
                Label postgreSQLArchLabel = new Label("What is the architecture of PostgreSQL that you want Hermes to be installed on?");

                HBox postgreSQLArchHBox = new HBox(10);
                postgreSQLArchHBox.getChildren().addAll(postgreSQLArchLabel, postgreSQLArch);

                grid.add(postgreSQLArchHBox, 0, 4);
            }
        } else {
            postgreSQLArch = new ChoiceBox(InstallerEnvironment.osArch.get().equals("x86") ? FXCollections.observableArrayList("32bit") : FXCollections.observableArrayList("64bit"));
            postgreSQLArch.setValue(InstallerEnvironment.osArch.get().equals("x86") ? "32bit" : "64bit");
        }

        postgreSQLArch.valueProperty().addListener((ObservableValue ov, Object t, Object t1) -> {
            //InstallerEnvironment.installPrerequisites.set(true);
        });

        //6th row
        Label thirdPartyLabel = new Label("Choose 3rd party components:");

        HBox thirdPartyHBox = new HBox(10);
        thirdPartyHBox.getChildren().addAll(thirdPartyLabel);

        grid.add(thirdPartyHBox, 0, 5);

        //7th row
        CheckBox postgis = new CheckBox("PostGIS");
        postgis.selectedProperty().bindBidirectional(InstallerEnvironment.postGIS);

        HBox postgisHBox = new HBox(10);
        postgisHBox.getChildren().addAll(postgis);

        grid.add(postgisHBox, 0, 6);

        //8th row
        CheckBox plpython = new CheckBox("PL/Python");
        plpython.selectedProperty().bindBidirectional(InstallerEnvironment.plpython);

        HBox plpythonHBox = new HBox(10);
        plpythonHBox.getChildren().addAll(plpython);

        grid.add(plpythonHBox, 0, 7);

        //9th row
        Label installAsLabel = new Label("Install as:");
        final TextField installAs = new TextField(InstallerEnvironment.installAs.get());

        HBox installAsHBox = new HBox(10);
        installAsHBox.getChildren().addAll(installAsLabel, installAs);

        grid.add(installAsHBox, 0, 9);

        Button back = new Button("Back");
        back.setOnAction((ActionEvent e) -> {
            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.licenseScene);
        });

        Button next = new Button("Next");
        next.setOnAction((ActionEvent e) -> {
            if (!installAs.getText().matches("^[_A-Za-z0-9]+$")) {
                PredefinedUIControls.showMessageDialog("\"Install as\" can contain only letters, numbers and underscore!");

                return;
            }

            InstallerEnvironment.installAs.set(installAs.getText());
            InstallerEnvironment.postgreSQLVersion.set((String) postgreSQLVersion.getValue());
            InstallerEnvironment.postgreSQLArch.set((String) postgreSQLArch.getValue());

            if (InstallerEnvironment.installPrerequisites.get()) {
                InstallerEnvironment.installPrerequisitesWindows();
                InstallerEnvironment.installPrerequisites.set(false);
            }

            InstallerEnvironment.findPostgreSQLInstallation();

            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.postgreSQLScene);
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(back, next);

        grid.add(buttonHBox, 1, 9);
    }
}
