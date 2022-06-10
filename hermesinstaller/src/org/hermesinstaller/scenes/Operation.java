package org.hermesinstaller.scenes;

import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class Operation extends Scene {

    public Operation(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Choose Operation"), 0, 2);

        //4th row
        Label message = new Label("What do you want to do today?");
        message.setWrapText(true);

        grid.add(message, 0, 3);

        //5th row
        ToggleGroup operations = new ToggleGroup();

        final RadioButton install = new RadioButton("Install Hermes");
        install.setToggleGroup(operations);
        install.setSelected(true);

        RadioButton update = new RadioButton("Update Hermes");
        update.setToggleGroup(operations);

        VBox operationsVBox = new VBox();
        operationsVBox.setSpacing(5);
        operationsVBox.getChildren().addAll(install, update);

        grid.add(operationsVBox, 0, 4);

        //6th row
        Button cancel = new Button("Cancel");
        cancel.setOnAction((ActionEvent e) -> {
            System.exit(0);
        });

        Button next = new Button("Next");
        next.setOnAction((ActionEvent e) -> {
            try {
                Class.forName("org.postgresql.Driver");
            } catch (ClassNotFoundException ex) {
                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            }

            if (install.isSelected()) {
                InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.versionScene);
            } else {
                InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.updateScene);
            }
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(cancel, next);

        grid.add(buttonHBox, 1, 5);
    }
}
