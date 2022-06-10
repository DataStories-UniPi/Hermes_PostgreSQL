package org.hermesinstaller;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class PredefinedUIControls {

    public static GridPane rootGrid() {
        GridPane grid = new GridPane();
        grid.setAlignment(Pos.CENTER);
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(25, 25, 25, 25));

        return grid;
    }

    public static HBox environmentInfo() {
        Label osName = new Label();
        osName.textProperty().bind(InstallerEnvironment.osName);

        Label osArch = new Label();
        osArch.textProperty().bind(InstallerEnvironment.osArch);

        Label osUser = new Label();
        osUser.textProperty().bind(InstallerEnvironment.osUser);

        Label version = new Label();
        version.textProperty().bind(InstallerEnvironment.installerDate);

        HBox hbox = new HBox(10);
        hbox.setAlignment(Pos.CENTER_RIGHT);
        hbox.getChildren().addAll(osName, osArch, osUser, version);

        return hbox;
    }

    public static HBox rootWarning() {
        HBox warningHBox = new HBox(10);
        warningHBox.setAlignment(Pos.CENTER_RIGHT);

        if ((InstallerEnvironment.osName.get().contains("Linux") || InstallerEnvironment.osName.get().contains("Mac OS X")) && !InstallerEnvironment.osUser.get().equals("root")) {
            Label warning = new Label("If installer fails, try running it as root");

            warningHBox.getChildren().addAll(warning);
        }

        return warningHBox;
    }

    public static Label decorateAsTitle(String string) {
        Label title = new Label(string);
        title.setFont(Font.font(Font.getDefault().getName(), FontWeight.NORMAL, 20));

        return title;
    }

    public static void showMessageDialog(String message) {
        final Stage dialog = new Stage(StageStyle.UTILITY);
        dialog.initOwner(InstallerEnvironment.primaryStage);
        dialog.initModality(Modality.WINDOW_MODAL);
        dialog.setTitle("Hermes Installer");

        GridPane grid = rootGrid();
        grid.add(decorateAsTitle("Message"), 0, 0);

        Label messageLabel = new Label(message);
        grid.add(messageLabel, 0, 1);

        Button okay = new Button("Okay");
        okay.setOnAction((ActionEvent e) -> {
            dialog.close();
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().add(okay);

        grid.add(buttonHBox, 1, 2);

        dialog.setScene(new Scene(grid));
        dialog.show();
    }
}
