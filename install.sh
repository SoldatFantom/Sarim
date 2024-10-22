 
#!/bin/bash
exec 2>/dev/null
# Vérifier si le script est exécuté avec les droits d'administrateur
if [ "$EUID" -ne 0 ]; then
    echo "Veuillez exécuter ce script avec sudo"
    exit 1
fi

echo "Installation de SF_PLAYER..."

# Installer les dépendances nécessaires

# Copier des fichiers dans les répertoires appropriés
cp sfplayer /usr/local/bin/sfplayer
chmod +x /usr/local/bin/sfplayer
echo "Installation terminée !"
rm output.log
