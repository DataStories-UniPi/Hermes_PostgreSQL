package org.hermesinstaller.scenes;

import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import org.hermesinstaller.PredefinedUIControls;

public class Unsupported extends Scene {

    public Unsupported(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Not Supported"), 0, 2);

        //4th row
        Label message = new Label("The Operating System you are using is not supported by this installer.");
        message.setWrapText(true);

        grid.add(message, 0, 3);

        //5th row
        Button close = new Button("Close");
        close.setOnAction((ActionEvent e) -> {
            System.exit(0);
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(close);

        grid.add(buttonHBox, 1, 4);
    }
}
