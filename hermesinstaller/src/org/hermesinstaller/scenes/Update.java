package org.hermesinstaller.scenes;

import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class Update extends Scene {

    public Update(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Update Hermes"), 0, 2);

        //4th row
        Label portLabel = new Label("PostgreSQL Port:");
        final TextField port = new TextField("5432");

        HBox portHBox = new HBox(10);
        portHBox.getChildren().addAll(portLabel, port);

        grid.add(portHBox, 0, 3);

        //5th row
        Label passwordLabel = new Label("PostgreSQL Password:");
        final PasswordField password = new PasswordField();

        HBox passwordHBox = new HBox(10);
        passwordHBox.getChildren().addAll(passwordLabel, password);

        grid.add(passwordHBox, 0, 4);

        //6th row
        Label message = new Label("Give the names of databases you want Hermes to be updated separated by comma (,).\n"
                + "Make sure that they all have the same version of Hermes, otherwise the operation will fail with unknown results.");
        message.setWrapText(true);

        grid.add(message, 0, 5);

        //7th row
        final TextField dbnames = new TextField("");

        grid.add(dbnames, 0, 6);

        //8th row
        Button cancel = new Button("Cancel");
        cancel.setOnAction((ActionEvent e) -> {
            System.exit(0);
            //InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.operationScene);
        });

        Button update = new Button("Update");
        update.setOnAction((ActionEvent e) -> {
            if (!port.getText().matches("^[0-9]+$")) {
                PredefinedUIControls.showMessageDialog("Give a valid port number!");

                return;
            }

            if (password.getText().equals("")) {
                PredefinedUIControls.showMessageDialog("Give a password!");

                return;
            }

            if (!dbnames.getText().matches("^[_,a-z0-9]+$")) {
                PredefinedUIControls.showMessageDialog("Spaces not allowed!");

                return;
            }

            InstallerEnvironment.postgreSQLPort.set(port.getText());
            InstallerEnvironment.postgreSQLPassword.set(password.getText());
            InstallerEnvironment.dbnames = dbnames.getText().split(",");

            if (InstallerEnvironment.updateHermes()) {
                if (!InstallerEnvironment.updateError) {
                    InstallerEnvironment.primaryStage.setScene(new UpdateSuccess(PredefinedUIControls.rootGrid()));
                }
            }
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(cancel, update);

        grid.add(buttonHBox, 1, 7);
    }
}
