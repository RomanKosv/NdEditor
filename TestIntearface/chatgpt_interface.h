#ifndef CHATGPT_INTERFACE_H
#define CHATGPT_INTERFACE_H
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QSlider>
#include <QToolBar>
#include <QMessageBox>

class NewModelWindow : public QDialog {
public:
    NewModelWindow(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLineEdit *directoryInput = new QLineEdit(this);
        QPushButton *browseButton = new QPushButton("Поиск", this);

        QLineEdit *fileNameInput = new QLineEdit(this);
        QPushButton *createFileButton = new QPushButton("Создать файл", this);

        layout->addWidget(new QLabel("Директория:"));
        layout->addWidget(directoryInput);
        layout->addWidget(browseButton);

        layout->addWidget(new QLabel("Имя файла:"));
        layout->addWidget(fileNameInput);

        layout->addWidget(createFileButton);

        connect(browseButton, &QPushButton::clicked, this, [=]() {
            QString dir = QFileDialog::getExistingDirectory(this, "Выберите директорию");
            directoryInput->setText(dir);
        });

        connect(createFileButton, &QPushButton::clicked, this, [=]() {
            // Логика создания файла
            QString filePath = directoryInput->text() + "/" + fileNameInput->text();
            // Создание файла по указанному пути
            accept();
        });
    }
};
class SettingsWindow : public QDialog {
public:
    SettingsWindow(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Пример настройки яркости
        QLabel *brightnessLabel = new QLabel("Яркость:");
        QSlider *brightnessSlider = new QSlider(Qt::Horizontal);

        layout->addWidget(brightnessLabel);
        layout->addWidget(brightnessSlider);

        // Добавь другие настройки (язык, масштаб и т.д.)

        QPushButton *saveSettingsBtn = new QPushButton("Сохранить", this);
        layout->addWidget(saveSettingsBtn);

        connect(saveSettingsBtn, &QPushButton::clicked, this, &SettingsWindow::accept);
    }
};
class WelcomeWindow : public QMainWindow {
    Q_OBJECT
public:
    WelcomeWindow() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        QPushButton *newModelButton = new QPushButton("Создать новую модель");
        QPushButton *openModelButton = new QPushButton("Открыть существующую модель");
        QPushButton *settingsButton = new QPushButton("Настройки");
        QPushButton *aboutButton = new QPushButton("О программе");

        layout->addWidget(newModelButton);
        layout->addWidget(openModelButton);
        layout->addWidget(settingsButton);
        layout->addWidget(aboutButton);

        connect(newModelButton, &QPushButton::clicked, this, &WelcomeWindow::createNewModel);
        connect(openModelButton, &QPushButton::clicked, this, &WelcomeWindow::openExistingModel);
        connect(settingsButton, &QPushButton::clicked, this, &WelcomeWindow::openSettings);
        connect(aboutButton, &QPushButton::clicked, this, &WelcomeWindow::showAbout);
    }

private slots:
    void createNewModel() {
        // Открыть окно создания новой модели
        NewModelWindow window;
        window.show();
    }

    void openExistingModel(){
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Файлы (*.model);;Все файлы (*)");
        if (!fileName.isEmpty()) {
            // Логика открытия файла
        }
    }

    void openSettings() {
        // Открыть окно настроек
        SettingsWindow window;
        window.show();
    }

    void showAbout() {
        // Показать информацию о программе
    }
};
/*
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WelcomeWindow window;
    window.show();

    return app.exec();
}
*/


class ModelEditor : public QMainWindow {
    Q_OBJECT
public:
    ModelEditor() {
        // Создание графического отображения модели (например через QGraphicsView)
        // Создание диспетчера объектов (например через QListView или QListWidget)
        // Создание верхней панели с кнопками

        // Пример верхней панели:
        QToolBar *toolbar = addToolBar("Toolbar");
        toolbar->addAction("Сохранить", this, &ModelEditor::saveModel);
        toolbar->addAction("Настройки", this, &ModelEditor::openSettings);
        toolbar->addAction("О программе", this, &ModelEditor::showAbout);

        // Добавь логику для диспетчера объектов и графического отображения модели.
    }

private slots:
    void saveModel() {
        // Логика сохранения модели
    }

    void openSettings() {
        SettingsWindow settings;
        settings.exec();
    }

    void showAbout() {
        QMessageBox::information(this, "О программе", "Информация о программе.");
    }
};

#endif // CHATGPT_INTERFACE_H
