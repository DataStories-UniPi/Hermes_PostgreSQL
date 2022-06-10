package org.hermesinstaller.scenes;

import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class Success extends Scene {

    public Success(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Installation Completed"), 0, 2);

        //4th row
        Label message = new Label("You have successfully installed Hermes on PostgreSQL!\n"
                + "\n"
                + "To create a new Hermes enabled database use: CREATE DATABASE dbname TEMPLATE=template_" + InstallerEnvironment.installAs.get().toLowerCase() + ";.\n"
                + "Don't modify template_" + InstallerEnvironment.installAs.get().toLowerCase() + " database or store data in it because in case you reinstall Hermes all changes will be lost.\n"
                + "\n"
                + "You can now close this installer.");
        message.setWrapText(true);

        grid.add(message, 0, 3);

        //5th row
        Button finish = new Button("Finish");
        finish.setOnAction((ActionEvent e) -> {
            System.exit(0);
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(finish);

        grid.add(buttonHBox, 1, 4);
    }
}
