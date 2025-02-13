# ECO TAXI
Eco Taxi — это программа, помогающая в записи доходов и расходов, а также для автоматизированного создания отчетности. В данной документации описаны основные процессы программы для получения отчетов, связанных с инвесторами, машинами, водителями, типами услуг и общий отчет, а также для управления данными программы и добавления новых записей.

# Eco Taxi - Installation and Setup Guide

## 1. Install Qt Framework

To run the project correctly, you need to install Qt Framework. Follow these steps:

1. Go to the official Qt website: [Qt Download](https://www.qt.io/download).
2. Register an account or log in.
3. Choose the free (Open Source) version.
4. Download and install Qt, following the installer instructions.
5. During installation, select the following components:
   - Qt version 6.7.1 or later
   - Qt Creator development environment
   - Additional tools such as MinGW compiler

For a visual guide, refer to the Qt installation tutorial.

---

## 2. Install and Set Up XAMPP

XAMPP is required for running the local server if the project uses a database.

### Steps to Install XAMPP:

1. Download XAMPP from the [official website](https://www.apachefriends.org/index.html).
2. Select the version for your operating system (Windows, macOS, Linux).
3. Run the installer and follow the setup instructions.
4. During installation, select required modules (Apache, MySQL, PHPMyAdmin).
5. After installation, open the XAMPP Control Panel and start **Apache** and **MySQL**.
6. Click the **Admin** button next to MySQL to open PHPMyAdmin.
7. Create a new database named `ecotaxidb`.

### Why XAMPP?

XAMPP provides a local server environment, making it easier to work with databases and backend components during development.

---

## 3. Install and Use GitHub Desktop

GitHub Desktop simplifies working with repositories through a graphical interface.

### Steps to Install GitHub Desktop:

1. Download GitHub Desktop from [GitHub Desktop](https://desktop.github.com).
2. Install the application by following the setup instructions.
3. Log in to your GitHub account or create a new one.

### How to Use GitHub Desktop:

1. Open GitHub Desktop and select **Clone a repository from the Internet**.
2. Enter the project repository link or select it from your repositories.
3. Choose a local directory and click **Clone**.
4. After cloning, the project will be available on your local computer.

### Making Changes:

1. Modify the project using your development environment.
2. Open GitHub Desktop, add the changes to a commit (**Commit changes** button).
3. Click **Push origin** to upload the changes to the remote repository.

Additional resources:

- [GitHub Documentation](https://docs.github.com/en/)
- [YouTube Tutorial](https://www.youtube.com)

---

## 4. Running the Project Locally

To run the project locally, follow these steps:

1. Open the project in **Qt Creator**.
2. Enable **Debug mode**.
3. Click **Run**.
4. If you encounter a database connection error, follow these steps:
   - Download and install the required database drivers: [MySQL Qt Driver](https://github.com/thecodemonkey86/qt_mysql_driver).
   - Extract the files (if archived) and move them to the project folder.
   - You can find the project folder in GitHub Desktop or Qt Creator (`Show in Explorer` in CMakeLists.txt).
   - The typical path: `{project folder}\build\Desktop_…_MinGW_64_bit-Debug`.
   - Run the project again.

By following these instructions, you can successfully set up and run the project on your local machine.
