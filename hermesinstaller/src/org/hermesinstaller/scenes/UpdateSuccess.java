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

public class UpdateSuccess extends Scene {

    public UpdateSuccess(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Update Completed"), 0, 2);

        //4th row
        Label message;
        if (InstallerEnvironment.updatedTo != null) {
            message = new Label("You have successfully updated Hermes to version " + InstallerEnvironment.updatedTo + "!\n"
                    + "\n"
                    + "You can now close this installer.");
        } else {
            message = new Label("There is no update available for your Hermes version.\n"
                    + "\n"
                    + "You can now close this installer.");
        }
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
