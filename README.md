Here’s how you can explain the installation steps in English for your README.md file:

markdown

# Installation of SFplayer

To install SFplayer, please follow the steps below:

## Prerequisites

Make sure you have Git installed on your system. You can check if Git is installed by running:

```bash
git --version

If Git is not installed, you can install it using the appropriate command for your operating system.
Installation Steps

    Clone the repository:

    Open a terminal and execute the following command to clone the repository:

    bash

git clone https://github.com/SoldatFantom/SFplayer.git

Navigate to the project directory:

Once the repository is cloned, navigate to the project directory:

bash

cd SFplayer

Make the installation script executable:

Before running the installation script, you need to make it executable with the following command:

bash

sudo chmod +x install.sh

Run the installation script:

Now, execute the installation script:

bash

    sudo ./install.sh

Verification of Installation

After the installation, you can verify if SFplayer has been installed correctly by running the following command:

bash

sfplayer --version

You should see the version of the program if the installation was successful.

Resume Installation Commands


git clone https://github.com/SoldatFantom/SFplayer.git
cd SFplayer
sudo chmod +x install.sh
sudo ./install.sh
sfplayer --version


