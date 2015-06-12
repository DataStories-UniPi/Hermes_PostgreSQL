# Installation {#installation}

[Hermes is compiled](https://hermes-mod.java.net/) for the following operating systems:

- Linux (64-bit only)
- Windows
- OS X

Hermes requires:

- [PostgreSQL  9.4](http://www.postgresql.org/)

# Installation on Ubuntu 14.04

- Install PostgreSQL. Postgresql 9.4 is not available in 14.04 "Trusty" but it was added in 14.10 "Utopic", so users that have 14.10 should only execute the last command. For the installation of the PostgreSQL [execute in terminal](http://www.postgresql.org/download/linux/ubuntu/):
  
      sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'
      sudo apt-get install wget ca-certificates
      wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
      sudo apt-get update      
      sudo apt-get upgrade
      sudo apt-get install postgresql-9.4 

	
- Copy the shared object (Hermes.so) file to your pgsql library directory. If you don't know where that is, check it out using: 

      $ pg_config --pkglibdir
      $ /usr/lib/postgresql/9.4/lib
      $ sudo cp Hermes.so 'pg_config --pkglibdir'


- Execute the script.installation to install the interface of Hermes. To execute it you must change to the postgres user. 

      $ sudo -u postgres psql -f script.installation	


# Installation on Windows

- Download the latest edition of [Ubuntu](http://www.ubuntu.com)
- Create a [bootable USB stick](http://www.ubuntu.com/download/desktop/create-a-usb-stick-on-windows)
- ERASE windows
- Install Ubuntu
- Follow the instruction of the previous section

Hopefully you wan`t commit suicide after realizing what a big mistake were Windows. Alternative you can use the [Hermes Installer](https://hermes-mod.java.net/Installer/BookVersion/HermesInstaller.zip)

# Installation on Mac OS

- If you have the money to buy a Mac pay someone that actually knows from computers to install it for you.

# Compile from source code on Ubuntu 14.04

If you are compiling the Hermes MOD from source except from PostgreSQL you will also need to install the postgresql-server-dev-9.4 

		sudo apt-get install postgresql-server-dev-9.4

Afterwards insert the project in [Eclispe](http://www.eclipse.org/cdt/) and try to compile it. There might be a possibility that the PostgreSQL was not detected and you will have to change the location of the library. Afterwards try compiling it until you don't get compiler-warnings anymore. 





