#include <user/config.h>

/*
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! LOCALISATION NOTES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    - Ensure brand names are always presented on the same line.

      Correct:
      This is a string that contains a brand name like
      Xbox 360, which is one of the two consoles to have a port of
      SONIC THE HEDGEHOG.

      Incorrect:
      This is a string that contains a brand name like Xbox
      360, which is one of the two consoles to have a port of SONIC THE
      HEDGEHOG.

    - Ensure your locale is added in the correct order following the language enum.

      Correct:
      {
          { ELanguage::English,  "Example" },
          { ELanguage::Japanese, "Example" },
          { ELanguage::German,   "Example" },
          { ELanguage::French,   "Example" },
          { ELanguage::Spanish,  "Example" },
          { ELanguage::Italian,  "Example" }
      }

      Incorrect:
      {
          { ELanguage::English,  "Example" },
          { ELanguage::French,   "Example" },
          { ELanguage::Spanish,  "Example" },
          { ELanguage::German,   "Example" },
          { ELanguage::Italian,  "Example" },
          { ELanguage::Japanese, "Example" }
      }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#define CONFIG_DEFINE_LOCALE(name) \
    CONFIG_LOCALE g_##name##_locale =

#define CONFIG_DEFINE_ENUM_LOCALE(type) \
    CONFIG_ENUM_LOCALE(type) g_##type##_locale =

CONFIG_DEFINE_LOCALE(Language)
{
    { ELanguage::English,  { "Language", "Change the language used for text." } },
    { ELanguage::Japanese, { "言語", "ゲーム内の表示言語を変更できます" } },
    { ELanguage::German,   { "Sprache", "Ändere die Textsprache." } },
    { ELanguage::French,   { "Langue", "Modifier la langue utilisée pour le texte." } },
    { ELanguage::Spanish,  { "Idioma", "Cambia el idioma utilizado para los textos." } },
    { ELanguage::Italian,  { "Lingua", "Cambia la lingua utilizzata per il testo." } }
};

// Notes: do not localise this.
CONFIG_DEFINE_ENUM_LOCALE(ELanguage)
{
    {
        ELanguage::English,
        {
            { ELanguage::English,  { "English", "" } },
            { ELanguage::Japanese, { "日本語", "" } },
            { ELanguage::German,   { "Deutsch", "" } },
            { ELanguage::French,   { "Français", "" } },
            { ELanguage::Spanish,  { "Español", "" } },
            { ELanguage::Italian,  { "Italiano", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(VoiceLanguage)
{
    { ELanguage::English, { "Voice Language", "Change the language used for character voices." } },
    { ELanguage::Japanese, { "音声言語", "ゲーム内の音声言語を変更できます" } },
    { ELanguage::German,   { "Stimmeinstellung", "Ändere die Sprache, die für Charakterstimmen benutzt wird." } },
    { ELanguage::French,   { "Langue de doublage", "Modifie la langue utilisée pour la voix des personnages." } },
    { ELanguage::Spanish,  { "Idioma de voz", "Cambia el idioma utilizado para las voces de los personajes." } },
    { ELanguage::Italian,  { "Lingua delle voci", "Modifica la lingua utilizzata per le voci dei personaggi." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EVoiceLanguage)
{
    {
        ELanguage::English,
        {
            { EVoiceLanguage::English,  { "English", "" } },
            { EVoiceLanguage::Japanese, { "Japanese", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EVoiceLanguage::English,  { "英語", "" } },
            { EVoiceLanguage::Japanese, { "日本語", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { EVoiceLanguage::English,  { "Englisch", "" } },
            { EVoiceLanguage::Japanese, { "Japanisch", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { EVoiceLanguage::English,  { "Anglais", "" } },
            { EVoiceLanguage::Japanese, { "Japonais", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { EVoiceLanguage::English,  { "Inglés", "" } },
            { EVoiceLanguage::Japanese, { "Japonés", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { EVoiceLanguage::English,  { "Inglese", "" } },
            { EVoiceLanguage::Japanese, { "Giapponese", "" } }
        }
    },
};

CONFIG_DEFINE_LOCALE(Subtitles)
{
    { ELanguage::English,  { "Subtitles", "Show subtitles during dialogue." } },
    { ELanguage::Japanese, { "字幕", "字幕の表示を選択できます" } },
    { ELanguage::German,   { "Untertitel", "Zeige Untertitel bei Dialogen." } },
    { ELanguage::French,   { "Sous-titres", "Affiche les sous-titres pendant les dialogues." } },
    { ELanguage::Spanish,  { "Subtítulos", "Muestra subtítulos durante los diálogos." } },
    { ELanguage::Italian,  { "Sottotitoli", "Mostra i sottotitoli durante i dialoghi." } }
};

CONFIG_DEFINE_LOCALE(Hints)
{
    { ELanguage::English,  { "Hints", "Show hints during gameplay." } },
    { ELanguage::Japanese, { "ヒントリング", "ゲーム内にヒントリングを表示するか選択できます" } },
    { ELanguage::German,   { "Hinweise", "Zeige Hinweise während des Spiels." } },
    { ELanguage::French,   { "Indices", "Affiche les indices pendant le jeu." } },
    { ELanguage::Spanish,  { "Pistas", "Muestra pistas durante el juego." } },
    { ELanguage::Italian,  { "Indizi", "Mostra degli indizzi durante il gioco." } }
};

CONFIG_DEFINE_LOCALE(ControlTutorial)
{
    { ELanguage::English,  { "Control Tutorial", "Show controller hints during gameplay." } },
    { ELanguage::Japanese, { "アクションナビ", "ゲーム内にアクションナビを表示するか選択できます" } },
    { ELanguage::German,   { "Steuerungsanleitung", "Zeige Steuerungshinweise während des Spiels." } },
    { ELanguage::French,   { "Indication des commandes", "Affiche les indications des commandes pendant le jeu." } },
    { ELanguage::Spanish,  { "Tutorial de controles", "Muestra pistas de controles durante el juego." } },
    { ELanguage::Italian,  { "Tutorial dei comandi", "Mostra i tutorial dei comandi durante il gioco." } }
};

CONFIG_DEFINE_LOCALE(Autosave)
{
    { ELanguage::English,  { "Autosave", "Save the game automatically at manual save points." } },
    { ELanguage::Japanese, { "オートセーブ", "手動セーブポイントでゲームを自動的にセーブします" } },
    { ELanguage::German,   { "Automatisches Speichern", "Speichert das Spiel automatisch an manuellen Speicherpunkten." } },
    { ELanguage::French,   { "Sauvegarde Auto", "Sauvegarder automatiquement la partie lorsque le jeu propose une sauvegarde." } },
    { ELanguage::Spanish,  { "Autoguardado", "Guarda el juego automáticamente en los puntos de guardado manuales." } },
    { ELanguage::Italian,  { "Salvataggio automatico", "Salva automaticamente il gioco nei punti di salvataggio manuali." } }
};

CONFIG_DEFINE_LOCALE(AchievementNotifications)
{
    { ELanguage::English,  { "Achievement Notifications", "Show notifications for unlocking achievements. Achievements will still be rewarded with notifications disabled." } },
    { ELanguage::Japanese, { "実績通知", "実績通知の有無を選択できます オフにしても実績は付与されます" } },
    { ELanguage::German,   { "Erfolgsbenachrichtigungen", "Zeige Benachrichtigungen für das Freischalten von Erfolgen. Erfolge werden weiterhin freigeschaltet, auch wenn die Benachrichtigungen ausgeschaltet sind." } },
    { ELanguage::French,   { "Notification des succès", "Affiche les notifications pour le déverrouillage des succès. Les succès seront toujours obtenus même si les notifications sont désactivées." } },
    { ELanguage::Spanish,  { "Notificaciones de logros", "Muestra notificaciones al desbloquear logros. Los logros se seguirán obteniendo aunque las notificaciones estén desactivadas." } },
    { ELanguage::Italian,  { "Notifiche obiettivi", "Mostra delle notifiche quando sblocchi degli obiettivi. Gli obiettivi verranno comunque assegnati anche con le notifiche disattivate." } }
};

CONFIG_DEFINE_LOCALE(HorizontalCamera)
{
    { ELanguage::English,  { "Horizontal Camera", "Change how the camera moves left and right." } },
    { ELanguage::Japanese, { "カメラの左右", "カメラ左右の動く方向を選択できます" } },
    { ELanguage::German,   { "Horizontale Kamera", "Ändere wie sich die Kamera nach links und rechts bewegt." } },
    { ELanguage::French,   { "Caméra horizontale", "Modifie la rotation horizontale de la caméra." } },
    { ELanguage::Spanish,  { "Cámara horizontal", "Cambia cómo se mueve la camara hacia la izquierda y la derecha." } },
    { ELanguage::Italian,  { "Telecamera orizzontale", "Modifica come la telecamera si muove da sinistra a destra." } }
};

CONFIG_DEFINE_LOCALE(VerticalCamera)
{
    { ELanguage::English,  { "Vertical Camera", "Change how the camera moves up and down." } },
    { ELanguage::Japanese, { "カメラの上下", "カメラ上下の動く方向を選択できます" } },
    { ELanguage::German,   { "Vertikale Kamera", "Ändere wie sich die Kamera nach oben und unten bewegt." } },
    { ELanguage::French,   { "Caméra verticale", "Modifie la rotation verticale de la caméra." } },
    { ELanguage::Spanish,  { "Cámara vertical", "Cambia cómo se mueve la camara hacia arriba y abajo." } },
    { ELanguage::Italian,  { "Telecamera verticale", "Modifica come la telecamera si muove su e giù." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ECameraRotationMode)
{
    {
        ELanguage::English,
        {
            { ECameraRotationMode::Normal,  { "Normal", "" } },
            { ECameraRotationMode::Reverse, { "Reverse", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { ECameraRotationMode::Normal,  { "ノーマル", "" } },
            { ECameraRotationMode::Reverse, { "リバース", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { ECameraRotationMode::Normal,  { "Normal", "" } },
            { ECameraRotationMode::Reverse, { "Invertiert", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { ECameraRotationMode::Normal,  { "Normale", "" } },
            { ECameraRotationMode::Reverse, { "Inversée", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { ECameraRotationMode::Normal,  { "Normal", "" } },
            { ECameraRotationMode::Reverse, { "Invertido", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { ECameraRotationMode::Normal,  { "Normale", "" } },
            { ECameraRotationMode::Reverse, { "Invertita", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(AllowBackgroundInput)
{
    { ELanguage::English,  { "Allow Background Input", "Allow controller input whilst the game window is unfocused." } },
    { ELanguage::Japanese, { "バックグラウンド入力", "フォーカスされていないゲームに入力できるか選択できます" } },
    { ELanguage::German,   { "Erlaube Hintergrundeingaben", "Erlaube Eingaben deines Controllers auch wenn das Spielfenster nicht fokussiert ist." } },
    { ELanguage::French,   { "Manette en arrière plan", "Permet d'utiliser la manette dans le jeu lorsque qu'il n'est pas au premier plan." } },
    { ELanguage::Spanish,  { "Control en segundo plano", "Permite controlar el juego con un mando mientras la ventana esté en segundo plano." } },
    { ELanguage::Italian,  { "Input con la finestra inattiva", "Attiva/disattiva i tasti del controller mentre la finestra è inattiva." } }
};

CONFIG_DEFINE_LOCALE(MouseSensitivityX)
{
    { ELanguage::English,  { "Mouse Sensitivity X", "Adjust horizontal mouse sensitivity." } },
    { ELanguage::Japanese, { "マウス感度 X", "水平マウス感度を調整します" } },
    { ELanguage::German,   { "Mausempfindlichkeit X", "Passe die horizontale Mausempfindlichkeit an." } },
    { ELanguage::French,   { "Sensibilité souris X", "Ajuste la sensibilité horizontale de la souris." } },
    { ELanguage::Spanish,  { "Sensibilidad del ratón X", "Ajusta la sensibilidad horizontal del ratón." } },
    { ELanguage::Italian,  { "Sensibilità mouse X", "Regola la sensibilità orizzontale del mouse." } }
};

CONFIG_DEFINE_LOCALE(MouseSensitivityY)
{
    { ELanguage::English,  { "Mouse Sensitivity Y", "Adjust vertical mouse sensitivity." } },
    { ELanguage::Japanese, { "マウス感度 Y", "垂直マウス感度を調整します" } },
    { ELanguage::German,   { "Mausempfindlichkeit Y", "Passe die vertikale Mausempfindlichkeit an." } },
    { ELanguage::French,   { "Sensibilité souris Y", "Ajuste la sensibilité verticale de la souris." } },
    { ELanguage::Spanish,  { "Sensibilidad del ratón Y", "Ajusta la sensibilidad vertical del ratón." } },
    { ELanguage::Italian,  { "Sensibilità mouse Y", "Regola la sensibilità verticale del mouse." } }
};

CONFIG_DEFINE_LOCALE(MouseInvertY)
{
    { ELanguage::English,  { "Invert Mouse Y", "Invert vertical mouse movement." } },
    { ELanguage::Japanese, { "マウス Y 反転", "垂直マウス移動を反転します" } },
    { ELanguage::German,   { "Maus Y invertieren", "Invertiere die vertikale Mausbewegung." } },
    { ELanguage::French,   { "Inverser souris Y", "Inverse le mouvement vertical de la souris." } },
    { ELanguage::Spanish,  { "Invertir ratón Y", "Invierte el movimiento vertical del ratón." } },
    { ELanguage::Italian,  { "Inverti mouse Y", "Inverti il movimento verticale del mouse." } }
};

CONFIG_DEFINE_LOCALE(MouseSmoothing)
{
    { ELanguage::English,  { "Mouse Smoothing", "Adjust mouse smoothing amount." } },
    { ELanguage::Japanese, { "マウススムージング", "マウススムージング量を調整します" } },
    { ELanguage::German,   { "Mausglättung", "Passe die Mausglättung an." } },
    { ELanguage::French,   { "Lissage souris", "Ajuste le lissage de la souris." } },
    { ELanguage::Spanish,  { "Suavizado del ratón", "Ajusta el suavizado del ratón." } },
    { ELanguage::Italian,  { "Smoothing mouse", "Regola lo smoothing del mouse." } }
};

CONFIG_DEFINE_LOCALE(ControllerIcons)
{
    { ELanguage::English,  { "Controller Icons", "Change the icons to match your controller." } },
    { ELanguage::Japanese, { "コントローラーアイコン", "ゲーム内のコントローラーアイコンを変更できます" } },
    { ELanguage::German,   { "Controllersymbole", "Ändere die Controllersymbole, um sie auf dein Modell anzupassen." } },
    { ELanguage::French,   { "Icône des boutons", "Modifie les icônes pour les faire correspondre à votre manette." } },
    { ELanguage::Spanish,  { "Iconos del mando", "Cambia los iconos para que coincidan con tu mando." } },
    { ELanguage::Italian,  { "Icone dei tasti", "Modifica le icone per farle corrispondere con il tuo controller." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EControllerIcons)
{
    {
        ELanguage::English,
        {
            { EControllerIcons::Auto,        { "Auto", "Auto: the game will determine which icons to use based on the current input device." } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EControllerIcons::Auto,        { "自動検出", "自動検出: コントローラーアイコンを使用している入力デバイスに応じて自動的に決定します" } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { EControllerIcons::Auto,        { "Auto", "Auto: Das Spiel erkennt automatisch deinen Controller um die Symbole dementsprechend anzupassen." } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { EControllerIcons::Auto,        { "Auto", "Auto : le jeu déterminera automatiquement quelles icônes utiliser en fonction du périphérique d'entrée." } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { EControllerIcons::Auto,        { "Auto", "Auto: el juego determinará de forma automática qué iconos utilizar dependiendo del dispositivo de entrada actual." } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { EControllerIcons::Auto,        { "Auto", "Auto: il gioco determinerà quali icone da utilizzare in base al dispositivo di input attuale." } },
            { EControllerIcons::Xbox,        { "Xbox", "" } },
            { EControllerIcons::PlayStation, { "PlayStation", "" } }
        }
    }
};
CONFIG_DEFINE_LOCALE(LightDash)
{
    { ELanguage::English,  { "Light Dash", "Change how Light Dash is activated for Sonic and Shadow." } },
    { ELanguage::Japanese, { "ライトダッシュ", "ソニックとシャドウのライトダッシュの発動方法を変更します" } },
    { ELanguage::German,   { "Lichtsprint", "Ändere wie der Lichtsprint von Sonic und Shadow aktiviert wird." } },
    { ELanguage::French,   { "Course éclair", "Modifier la façon dont Course éclair est activée pour Sonic et Shadow." } },
    { ELanguage::Spanish,  { "Acelerón ligero", "Cambia cómo se activa el acelerón ligero para Sonic y Shadow." } },
    { ELanguage::Italian,  { "Super spinta", "Cambia il modo in cui viene attivata la Super spinta per Sonic e Shadow." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ELightDash)
{
    {
        ELanguage::English,
        {
            { ELightDash::X, { "Press ${picture(button_x)}", "" } },
            { ELightDash::Y, { "Press ${picture(button_y)}", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { ELightDash::X, { "${picture(button_x)}を押す", "" } },
            { ELightDash::Y, { "${picture(button_y)}を押す", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { ELightDash::X, { "Drücke ${picture(button_x)}", "" } },
            { ELightDash::Y, { "Drücke ${picture(button_y)}", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { ELightDash::X, { "Appuyer sur ${picture(button_x)}", "" } },
            { ELightDash::Y, { "Appuyer sur ${picture(button_y)}", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { ELightDash::X, { "Pulsar ${picture(button_x)}", "" } },
            { ELightDash::Y, { "Pulsar ${picture(button_y)}", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { ELightDash::X, { "Premi ${picture(button_x)}", "" } },
            { ELightDash::Y, { "Premi ${picture(button_y)}", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(SlidingAttack)
{
    { ELanguage::English,  { "Sliding Attack", "Change how the Sliding Attack is activated for Sonic." } },
    { ELanguage::Japanese, { "スライディングアタック", "ソニックのスライディングアタックの発動方法を変更します" } },
    { ELanguage::German,   { "Schlitterangriff", "Ändere wie der Schlitterangriff von Sonic aktiviert wird." } },
    { ELanguage::French,   { "Attaque-dérapage", "Modifier la façon dont l'Attaque-dérapage est activée pour Sonic." } },
    { ELanguage::Spanish,  { "Ataque derrape", "Cambia cómo se activa el ataque derrape para Sonic." } },
    { ELanguage::Italian,  { "Scivolata", "Cambia il modo in cui viene attivata la Scivolata per Sonic." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ESlidingAttack)
{
    {
        ELanguage::English,
        {
            { ESlidingAttack::B, { "Hold ${picture(button_b)}", "" } },
            { ESlidingAttack::X, { "Release ${picture(button_x)}", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { ESlidingAttack::B, { "${picture(button_b)}を押し続ける", "" } },
            { ESlidingAttack::X, { "${picture(button_x)}を離す", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { ESlidingAttack::B, { "Halte ${picture(button_b)} gedrückt", "" } },
            { ESlidingAttack::X, { "Lasse ${picture(button_x)} los", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { ESlidingAttack::B, { "Maintenir ${picture(button_b)}", "" } },
            { ESlidingAttack::X, { "Relâcher ${picture(button_x)}", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { ESlidingAttack::B, { "Mantener ${picture(button_b)}", "" } },
            { ESlidingAttack::X, { "Soltar ${picture(button_x)}", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { ESlidingAttack::B, { "Tieni premuto ${picture(button_b)}", "" } },
            { ESlidingAttack::X, { "Rilascia ${picture(button_x)}", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(MasterVolume)
{
    { ELanguage::English,  { "Master Volume", "Adjust the overall volume." } },
    { ELanguage::Japanese, { "マスターボリューム", "全体ボリュームの大きさを調整できます" } },
    { ELanguage::German,   { "Gesamtlautstärke", "Passe die Gesamtlautstärke an." } },
    { ELanguage::French,   { "Volume général", "Réglage du volume général." } },
    { ELanguage::Spanish,  { "Volumen maestro", "Ajusta el volumen general." } },
    { ELanguage::Italian,  { "Volume principale", "Regola il volume principale" } }
};

CONFIG_DEFINE_LOCALE(MusicVolume)
{
    { ELanguage::English,  { "Music Volume", "Adjust the volume for the music." } },
    { ELanguage::Japanese, { "BGMボリューム", "BGMボリュームの大きさを調整できます" } },
    { ELanguage::German,   { "Musiklautstärke", "Passe die Lautstärke der Musik an." } },
    { ELanguage::French,   { "Volume de la musique", "Réglage du volume de la musique." } },
    { ELanguage::Spanish,  { "Volumen de la música", "Ajusta el volumen de la música." } },
    { ELanguage::Italian,  { "Volume musica di sottofondo", "Regola il volume della musica di sottofondo." } }
};

CONFIG_DEFINE_LOCALE(EffectsVolume)
{
    { ELanguage::English,  { "Effects Volume", "Adjust the volume for sound effects." } },
    { ELanguage::Japanese, { "SEボリューム", "SEボリュームの大きさを調整できます" } },
    { ELanguage::German,   { "Soundeffektlautstärke", "Passe die Lautstärke der Soundeffekte an." } },
    { ELanguage::French,   { "Volume des effets sonores", "Réglage du volume des effets sonores." } },
    { ELanguage::Spanish,  { "Volumen de efectos", "Ajusta el volumen de los efectos de sonido." } },
    { ELanguage::Italian,  { "Volume effetti sonori", "Regola il volume degli effetti sonori." } }
};

CONFIG_DEFINE_LOCALE(ChannelConfiguration)
{
    { ELanguage::English,  { "Channel Configuration", "Change the output mode for your audio device." } },
    { ELanguage::Japanese, { "チャンネル設定", "オーディオデバイスの出力モードを変更できます" } },
    { ELanguage::German,   { "Kanalkonfiguration", "Ändere den Ausgabemodus für dein Audioausgabegerät." } },
    { ELanguage::French,   { "Configuration sortie audio", "Modifie le mode de sortie pour votre périphérique audio." } },
    { ELanguage::Spanish,  { "Configuración de canales", "Cambia el modo de salida para tu dispositivo de audio." } },
    { ELanguage::Italian,  { "Configurazione canali audio", "Modifica la modalità di output per il tuo dispositivo audio." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EChannelConfiguration)
{
    {
        ELanguage::English,
        {
            { EChannelConfiguration::Stereo,   { "Stereo", "" } },
            { EChannelConfiguration::Surround, { "Surround", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EChannelConfiguration::Stereo,   { "ステレオ", "" } },
            { EChannelConfiguration::Surround, { "サラウンド", "" } }
        }
    },
    {
        ELanguage::German,
        {
            { EChannelConfiguration::Stereo,   { "Stereo", "" } },
            { EChannelConfiguration::Surround, { "Surround", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { EChannelConfiguration::Stereo,   { "Stéréo", "" } },
            { EChannelConfiguration::Surround, { "Surround", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { EChannelConfiguration::Stereo,   { "Estéreo", "" } },
            { EChannelConfiguration::Surround, { "Envolvente", "" } }
        }
    },
    {
        ELanguage::Italian,
        {
            { EChannelConfiguration::Stereo,   { "Stereo", "" } },
            { EChannelConfiguration::Surround, { "Surround", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(MuteOnFocusLost)
{
    { ELanguage::English,  { "Mute on Focus Lost", "Mute the game's audio when the window is not in focus." } },
    { ELanguage::Japanese, { "フォーカスロストミュート", "ウィンドウがフォーカスされていないときにゲームのオーディオをミュートします" } },
    { ELanguage::German,   { "Stummstellen wenn nicht fokussiert", "Stellt das Audio des Spiels stumm solange das Fenster nicht im Fokus ist." } },
    { ELanguage::French,   { "Muet si fenêtre inactive", "Coupe le son du jeu lorsque la fenêtre n'est pas active." } },
    { ELanguage::Spanish,  { "Silenciar al perder el foco", "Silencia el audio del juego cuando la ventana no esté en primer plano." } },
    { ELanguage::Italian,  { "Audio muto con finestra inattiva", "Disattiva l'audio del gioco quando la finestra non è attiva." } }
};

CONFIG_DEFINE_LOCALE(MusicAttenuation)
{
    { ELanguage::English,  { "Music Attenuation", "Fade out the game's music when external media is playing." } },
    { ELanguage::Japanese, { "BGM減衰", "外部メディアを再生するとゲームの音楽をフェードアウトします" } },
    { ELanguage::German,   { "Musikdämpfung", "Stelle die Musik des Spiels stumm während externe Medien abgespielt werden." } },
    { ELanguage::French,   { "Atténuation audio", "Abaisse le volume des musiques du jeu lorsqu'un média externe est en cours de lecture." } },
    { ELanguage::Spanish,  { "Atenuación de música", "Atenúa la música del juego cuando un reproductor multimedia se encuentra activo." } },
    { ELanguage::Italian,  { "Attenuazione musica", "Abbassa il volume della musica di sottofondo quando un'altra applicazione riproduce dei suoni." } }
};

CONFIG_DEFINE_LOCALE(WindowSize)
{
    { ELanguage::English,  { "Window Size", "Adjust the size of the game window in windowed mode." } },
    { ELanguage::Japanese, { "ウィンドウサイズ", "ウィンドウモードでのゲームのウィンドウサイズを調整できます" } },
    { ELanguage::German,   { "Fenstergröße", "Ändere die Größe des Spielfensters im Fenstermodus." } },
    { ELanguage::French,   { "Taille de la fenêtre", "Modifie la taille de la fenêtre de jeu en mode fenêtré." } },
    { ELanguage::Spanish,  { "Tamaño de ventana", "Ajusta el tamaño de la ventana de juego." } },
    { ELanguage::Italian,  { "Dimensioni finestra", "Regola le dimensioni della finestra del gioco in modalità finestra." } }
};

CONFIG_DEFINE_LOCALE(Monitor)
{
    { ELanguage::English,  { "Monitor", "Change which monitor to display the game on." } },
    { ELanguage::Japanese, { "モニター選択", "ゲームを表示するモニターを変更できます" } },
    { ELanguage::German,   { "Monitor", "Ändere auf welchem Monitor das Spiel angezeigt wird." } },
    { ELanguage::French,   { "Moniteur", "Change le moniteur sur lequel le jeu sera affiché." } },
    { ELanguage::Spanish,  { "Pantalla", "Cambia la pantalla en la cuál se muestra el juego." } },
    { ELanguage::Italian,  { "Schermo", "Cambia lo schermo su cui visualizzare il gioco." } }
};

CONFIG_DEFINE_LOCALE(AspectRatio)
{
    { ELanguage::English,  { "Aspect Ratio", "Change the aspect ratio." } },
    { ELanguage::Japanese, { "アスペクト比", "アスペクト比を変更できます" } },
    { ELanguage::German,   { "Seitenverhältnis", "Verändere das Seitenverhältnis." } },
    { ELanguage::French,   { "Format d'image", "Modifie le format d'image." } },
    { ELanguage::Spanish,  { "Relación de aspecto", "Cambia la relación de aspecto." } },
    { ELanguage::Italian,  { "Rapporto d'aspetto", "Modifica il rapporto d'aspetto." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EAspectRatio)
{
    {
        ELanguage::English,
        {
            { EAspectRatio::Auto,     { "Auto", "Auto: the aspect ratio will dynamically adjust to the window size." } },
            { EAspectRatio::Original, { "Original", "Original: locks the game to a widescreen aspect ratio." } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EAspectRatio::Auto,     { "自動", "自動: アスペクト比はウィンドウサイズに合わせて調整されます" } },
            { EAspectRatio::Original, { "オリジナル", "オリジナル: ワイドスクリーンのアスペクト比に固定されます" } }
        }
    },
    {
        ELanguage::German,
        {
            { EAspectRatio::Auto,     { "Auto", "Auto: Das Seitenverhältnis passt sich automatisch der Fenstergröße an." } },
            { EAspectRatio::Original, { "Original", "Original: Stellt das Spiel in einem Breitbildschirm-Format dar." } }
        }
    },
    {
        ELanguage::French,
        {
            { EAspectRatio::Auto,     { "Auto", "Auto : le format d'image s'adapte automatiquement à la taille de la fenêtre." } },
            { EAspectRatio::Original, { "Original", "Original : force le jeu sur un format d'image large." } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { EAspectRatio::Auto,     { "Auto", "Auto: la relación de aspecto se ajusta de forma dinámica al tamaño de la ventana." } },
            { EAspectRatio::Original, { "Original", "Original: muestra el juego en relación de aspecto de pantalla ancha." } }
        }
    },
    {
        ELanguage::Italian,
        {
            { EAspectRatio::Auto,     { "Auto", "Auto: il rapporto d'aspetto verra cambiato automaticamente in base alle dimensioni della finestra." } },
            { EAspectRatio::Original, { "Originale", "Originale: blocca il gioco a un rapporto d'aspetto widescreen." } }
        }
    }
};

CONFIG_DEFINE_LOCALE(ResolutionScale)
{
    { ELanguage::English,  { "Resolution Scale", "Adjust the internal resolution of the game." } },
    { ELanguage::Japanese, { "解像度スケール", "ゲームの内部解像度を調整できます" } },
    { ELanguage::German,   { "Rendering-Auflösung", "Passe die Auflösung der internen Darstellung an." } },
    { ELanguage::French,   { "Échelle de rendu", "Modifie la résolution interne du jeu." } },
    { ELanguage::Spanish,  { "Escala de resolución", "Ajusta la resolución interna del juego." } },
    { ELanguage::Italian,  { "Scala risoluzione", "Regola la risoluzione interna del gioco." } }
};

CONFIG_DEFINE_LOCALE(Fullscreen)
{
    { ELanguage::English,  { "Fullscreen", "Toggle between borderless fullscreen or windowed mode." } },
    { ELanguage::Japanese, { "フルスクリーン", "ボーダーレスフルスクリーンかウィンドウモードを選択できます" } },
    { ELanguage::German,   { "Vollbild", "Wechsle zwischen dem randlosen Vollbildmodus und dem Fenstermodus." } },
    { ELanguage::French,   { "Plein écran", "Alterne entre le mode plein écran sans bordures et le mode fenêtré." } },
    { ELanguage::Spanish,  { "Pantalla completa", "Cambia entre modo de pantalla completa o ventana." } },
    { ELanguage::Italian,  { "Schermo pieno", "Attiva/disattiva tra modalità finestra senza cornice e modalità finestra." } }
};

CONFIG_DEFINE_LOCALE(VSync)
{
    { ELanguage::English,  { "V-Sync", "Synchronize the game to the refresh rate of the display to prevent screen tearing." } },
    { ELanguage::Japanese, { "垂直同期", "垂直同期の設定を変更できます" } },
    { ELanguage::German,   { "V-Sync", "Synchronisiere das Spiel mit der Bildwiederholrate deines Bildschirms um Bildverzerrungen zu vermeiden." } },
    { ELanguage::French,   { "V-Sync", "Synchronise le jeu avec la fréquence de rafraîchissement de l'écran pour éviter le screen tearing." } },
    { ELanguage::Spanish,  { "V-Sync", "Sincroniza el juego a la tasa de refresco de la pantalla para prevenir el rasgado de la imagen." } },
    { ELanguage::Italian,  { "V-Sync", "Sincronizza il gioco con la frequenza d'aggiornamento del display per evitare lo screen tearing." } }
};

CONFIG_DEFINE_LOCALE(FPS)
{
    { ELanguage::English,  { "FPS", "Set the max frame rate the game can run at. WARNING: this may introduce glitches at frame rates other than 60 FPS." } },
    { ELanguage::Japanese, { "フレームレート上限", "ゲームが実行できる最大フレームレートを設定します　警告:60FPS以外のフレームレートでは不具合が発生する可能性があります" } },
    { ELanguage::German,   { "FPS", "Bestimmt die maximale Bildwiederholrate. WARNUNG: es können Fehler bei einer Bildwiederholrate über 60 FPS auftreten." } },
    { ELanguage::French,   { "IPS", "Limiter le nombre d'images par seconde. ATTENTION : Cela peut provoquer des bugs à des fréquences autres que 60 IPS." } },
    { ELanguage::Spanish,  { "FPS", "Establece la tasa máxima de fotogramas a la que puede ejecutarse el juego. ADVERTENCIA: esto puede provocar fallos en velocidades distintas a 60 FPS." } },
    { ELanguage::Italian,  { "FPS", "Imposta il framerate massimo del gioco. ATTENZIONE: questa opzione può causare problemi con dei framerate rate superiori a 60 FPS." } }
};

CONFIG_DEFINE_LOCALE(Brightness)
{
    { ELanguage::English,  { "Brightness", "Adjust the brightness level." } },
    { ELanguage::Japanese, { "明るさの設定", "明るさレベルを調整します" } },
    { ELanguage::German,   { "Helligkeit", "Stelle die Helligkeit ein." } },
    { ELanguage::French,   { "Luminosité", "Modifier le niveau de luminosité." } },
    { ELanguage::Spanish,  { "Brillo", "Ajusta el nivel de brillo." } },
    { ELanguage::Italian,  { "Luminosità", "Regola il livello di luminosità." } }
};

CONFIG_DEFINE_LOCALE(HDRMode)
{
    { ELanguage::English,  { "HDR Mode", "Enable HDR output for compatible displays. scRGB for Windows HDR, HDR10 for TVs." } },
    { ELanguage::Japanese, { "HDRモード", "対応ディスプレイでHDR出力を有効にします" } },
    { ELanguage::German,   { "HDR-Modus", "HDR-Ausgabe für kompatible Displays aktivieren." } },
    { ELanguage::French,   { "Mode HDR", "Activer la sortie HDR pour les écrans compatibles." } },
    { ELanguage::Spanish,  { "Modo HDR", "Habilitar salida HDR para pantallas compatibles." } },
    { ELanguage::Italian,  { "Modalità HDR", "Abilita l'output HDR per display compatibili." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EHDRMode)
{
    {
        ELanguage::English,
        {
            { EHDRMode::Off,   { "Off", "" } },
            { EHDRMode::scRGB, { "scRGB", "" } },
            { EHDRMode::HDR10, { "HDR10", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(HDRPaperWhite)
{
    { ELanguage::English,  { "Paper White", "HDR brightness for standard content (nits). Default: 200." } },
    { ELanguage::Japanese, { "ペーパーホワイト", "標準コンテンツのHDR輝度（ニト）" } },
    { ELanguage::German,   { "Papierweiss", "HDR-Helligkeit für Standardinhalte (Nits)." } },
    { ELanguage::French,   { "Blanc papier", "Luminosité HDR pour le contenu standard (nits)." } },
    { ELanguage::Spanish,  { "Blanco papel", "Brillo HDR para contenido estándar (nits)." } },
    { ELanguage::Italian,  { "Bianco carta", "Luminosità HDR per contenuti standard (nits)." } }
};

CONFIG_DEFINE_LOCALE(HDRMaxLuminance)
{
    { ELanguage::English,  { "Max Luminance", "Maximum HDR brightness for highlights (nits). Default: 1000." } },
    { ELanguage::Japanese, { "最大輝度", "ハイライトの最大HDR輝度（ニト）" } },
    { ELanguage::German,   { "Max. Helligkeit", "Maximale HDR-Helligkeit für Highlights (Nits)." } },
    { ELanguage::French,   { "Luminance max", "Luminosité HDR maximale pour les reflets (nits)." } },
    { ELanguage::Spanish,  { "Luminancia máx", "Brillo HDR máximo para reflejos (nits)." } },
    { ELanguage::Italian,  { "Luminanza max", "Luminosità HDR massima per highlights (nits)." } }
};

CONFIG_DEFINE_LOCALE(ModernAA)
{
    { ELanguage::English,  { "Modern AA", "Select modern anti-aliasing technique. TAA for temporal, SMAA for morphological, FSR1 for AMD upscaling as AA." } },
    { ELanguage::Japanese, { "モダンAA", "最新のアンチエイリアス技術を選択します" } },
    { ELanguage::German,   { "Modernes AA", "Wähle moderne Anti-Aliasing-Technik." } },
    { ELanguage::French,   { "AA Moderne", "Sélectionner une technique d'anticrénelage moderne." } },
    { ELanguage::Spanish,  { "AA Moderno", "Seleccionar técnica de anti-aliasing moderna." } },
    { ELanguage::Italian,  { "AA Moderno", "Seleziona tecnica di anti-aliasing moderna." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EModernAA)
{
    {
        ELanguage::English,
        {
            { EModernAA::Off,  { "Off", "" } },
            { EModernAA::TAA,  { "TAA", "" } },
            { EModernAA::SMAA, { "SMAA", "" } },
            { EModernAA::FSR1, { "FSR 1.0", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(DynamicResolution)
{
    { ELanguage::English,  { "Dynamic Resolution", "Automatically adjust internal resolution to maintain target framerate." } },
    { ELanguage::Japanese, { "動的解像度", "目標フレームレートを維持するために内部解像度を自動調整します" } },
    { ELanguage::German,   { "Dynamische Auflösung", "Interne Auflösung automatisch anpassen." } },
    { ELanguage::French,   { "Résolution Dynamique", "Ajuster automatiquement la résolution interne." } },
    { ELanguage::Spanish,  { "Resolución Dinámica", "Ajustar automáticamente la resolución interna." } },
    { ELanguage::Italian,  { "Risoluzione Dinamica", "Regola automaticamente la risoluzione interna." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EDynamicResolution)
{
    {
        ELanguage::English,
        {
            { EDynamicResolution::Off,         { "Off", "" } },
            { EDynamicResolution::Quality,     { "Quality", "" } },
            { EDynamicResolution::Balanced,    { "Balanced", "" } },
            { EDynamicResolution::Performance, { "Performance", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(MinResolutionScale)
{
    { ELanguage::English,  { "Min Resolution Scale", "Minimum resolution scale for dynamic resolution (0.5 = 50%)." } },
    { ELanguage::Japanese, { "最小解像度スケール", "動的解像度の最小スケール" } },
    { ELanguage::German,   { "Min. Auflösungsskala", "Minimale Auflösungsskalierung für dynamische Auflösung." } },
    { ELanguage::French,   { "Échelle Min", "Échelle minimale pour la résolution dynamique." } },
    { ELanguage::Spanish,  { "Escala Mín", "Escala mínima para resolución dinámica." } },
    { ELanguage::Italian,  { "Scala Min", "Scala minima per risoluzione dinamica." } }
};

CONFIG_DEFINE_LOCALE(TargetFrameTime)
{
    { ELanguage::English,  { "Target Frame Time", "Target frame time in milliseconds for dynamic resolution (16.67ms = 60fps)." } },
    { ELanguage::Japanese, { "目標フレーム時間", "動的解像度の目標フレーム時間（ミリ秒）" } },
    { ELanguage::German,   { "Ziel-Framezeit", "Ziel-Framezeit in Millisekunden für dynamische Auflösung." } },
    { ELanguage::French,   { "Temps Cible", "Temps de frame cible en millisecondes." } },
    { ELanguage::Spanish,  { "Tiempo Objetivo", "Tiempo de frame objetivo en milisegundos." } },
    { ELanguage::Italian,  { "Tempo Target", "Tempo di frame target in millisecondi." } }
};

CONFIG_DEFINE_LOCALE(MotionBlur)
{
    { ELanguage::English,  { "Motion Blur", "Modern per-pixel motion blur using velocity buffers." } },
    { ELanguage::Japanese, { "モーションブラー", "速度バッファを使用したモダンなモーションブラー" } },
    { ELanguage::German,   { "Bewegungsunschärfe", "Moderne Bewegungsunschärfe mit Geschwindigkeitspuffer." } },
    { ELanguage::French,   { "Flou de Mouvement", "Flou de mouvement moderne avec buffer de vélocité." } },
    { ELanguage::Spanish,  { "Desenfoque de Movimiento", "Desenfoque de movimiento moderno con buffer de velocidad." } },
    { ELanguage::Italian,  { "Motion Blur", "Motion blur moderno con buffer di velocità." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EMotionBlur)
{
    {
        ELanguage::English,
        {
            { EMotionBlur::Off,      { "Off", "" } },
            { EMotionBlur::Camera,   { "Camera Only", "" } },
            { EMotionBlur::Enhanced, { "Enhanced", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(MotionBlurStrength)
{
    { ELanguage::English,  { "Motion Blur Strength", "Intensity of the motion blur effect (0.0-2.0)." } },
    { ELanguage::Japanese, { "モーションブラー強度", "モーションブラーの強度" } },
    { ELanguage::German,   { "Bewegungsunschärfe-Stärke", "Intensität der Bewegungsunschärfe." } },
    { ELanguage::French,   { "Intensité du Flou", "Intensité de l'effet de flou de mouvement." } },
    { ELanguage::Spanish,  { "Intensidad del Desenfoque", "Intensidad del efecto de desenfoque." } },
    { ELanguage::Italian,  { "Intensità Motion Blur", "Intensità dell'effetto motion blur." } }
};

CONFIG_DEFINE_LOCALE(SSAA)
{
    { ELanguage::English,  { "Super-Sampling", "Render at higher resolution and downsample for best quality AA. Very demanding." } },
    { ELanguage::Japanese, { "スーパーサンプリング", "高解像度でレンダリングし、ダウンサンプリングで最高品質のAA。非常に負荷が高い。" } },
    { ELanguage::German,   { "Super-Sampling", "Rendert in höherer Auflösung für beste AA-Qualität. Sehr anspruchsvoll." } },
    { ELanguage::French,   { "Super-Échantillonnage", "Rendu en haute résolution pour la meilleure qualité AA. Très exigeant." } },
    { ELanguage::Spanish,  { "Super-Muestreo", "Renderiza a mayor resolución para la mejor calidad AA. Muy exigente." } },
    { ELanguage::Italian,  { "Super-Sampling", "Rendering a risoluzione maggiore per la migliore qualità AA. Molto esigente." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ESSAA)
{
    {
        ELanguage::English,
        {
            { ESSAA::Off,  { "Off", "" } },
            { ESSAA::x1_5, { "1.5x", "" } },
            { ESSAA::x2,   { "2x", "" } },
            { ESSAA::x4,   { "4x", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(DepthOfField)
{
    { ELanguage::English,  { "Depth of Field", "Blur objects based on distance from focus point." } },
    { ELanguage::Japanese, { "被写界深度", "フォーカスポイントからの距離に基づいてオブジェクトをぼかします。" } },
    { ELanguage::German,   { "Tiefenschärfe", "Objekte basierend auf Entfernung zum Fokuspunkt weichzeichnen." } },
    { ELanguage::French,   { "Profondeur de Champ", "Flouter les objets selon la distance au point focal." } },
    { ELanguage::Spanish,  { "Profundidad de Campo", "Desenfocar objetos según distancia al punto focal." } },
    { ELanguage::Italian,  { "Profondità di Campo", "Sfoca gli oggetti in base alla distanza dal punto focale." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EDepthOfField)
{
    {
        ELanguage::English,
        {
            { EDepthOfField::Off,    { "Off", "" } },
            { EDepthOfField::Low,    { "Low", "" } },
            { EDepthOfField::Medium, { "Medium", "" } },
            { EDepthOfField::High,   { "High", "" } },
            { EDepthOfField::Ultra,  { "Ultra", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(DOFFocusDistance)
{
    { ELanguage::English,  { "Focus Distance", "Distance to the focus plane in meters." } },
    { ELanguage::Japanese, { "フォーカス距離", "フォーカス面までの距離（メートル）" } },
    { ELanguage::German,   { "Fokusabstand", "Abstand zur Fokusebene in Metern." } },
    { ELanguage::French,   { "Distance Focale", "Distance au plan focal en mètres." } },
    { ELanguage::Spanish,  { "Distancia Focal", "Distancia al plano de enfoque en metros." } },
    { ELanguage::Italian,  { "Distanza Focale", "Distanza dal piano focale in metri." } }
};

CONFIG_DEFINE_LOCALE(DOFApertureSize)
{
    { ELanguage::English,  { "Aperture Size", "Size of the lens aperture (smaller = sharper, larger = more blur)." } },
    { ELanguage::Japanese, { "絞りサイズ", "レンズ絞りのサイズ（小=シャープ、大=ぼかし）" } },
    { ELanguage::German,   { "Blendengröße", "Größe der Linsenblende (kleiner = schärfer)." } },
    { ELanguage::French,   { "Ouverture", "Taille de l'ouverture (petit = net, grand = flou)." } },
    { ELanguage::Spanish,  { "Apertura", "Tamaño de la apertura (pequeño = nítido, grande = borroso)." } },
    { ELanguage::Italian,  { "Apertura", "Dimensione dell'apertura (piccolo = nitido, grande = sfocato)." } }
};

CONFIG_DEFINE_LOCALE(SSAO)
{
    { ELanguage::English,  { "SSAO", "Screen-Space Ambient Occlusion adds realistic shadows in corners and crevices." } },
    { ELanguage::Japanese, { "SSAO", "スクリーンスペースアンビエントオクルージョン。角や隙間にリアルな影を追加します。" } },
    { ELanguage::German,   { "SSAO", "Bildschirmbasierte Umgebungsverdeckung für realistische Schatten." } },
    { ELanguage::French,   { "SSAO", "Occlusion ambiante en espace écran pour des ombres réalistes." } },
    { ELanguage::Spanish,  { "SSAO", "Oclusión ambiental en espacio de pantalla para sombras realistas." } },
    { ELanguage::Italian,  { "SSAO", "Occlusione ambientale nello spazio dello schermo per ombre realistiche." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ESSAO)
{
    {
        ELanguage::English,
        {
            { ESSAO::Off,    { "Off", "" } },
            { ESSAO::Low,    { "Low", "" } },
            { ESSAO::Medium, { "Medium", "" } },
            { ESSAO::High,   { "High", "" } },
            { ESSAO::Ultra,  { "Ultra", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(SSAORadius)
{
    { ELanguage::English,  { "SSAO Radius", "World-space radius for ambient occlusion sampling." } }
};

CONFIG_DEFINE_LOCALE(SSAOIntensity)
{
    { ELanguage::English,  { "SSAO Intensity", "Strength of the ambient occlusion effect." } }
};

CONFIG_DEFINE_LOCALE(SSAOBias)
{
    { ELanguage::English,  { "SSAO Bias", "Depth comparison bias to prevent self-occlusion artifacts." } }
};

CONFIG_DEFINE_LOCALE(SSAOFalloffDistance)
{
    { ELanguage::English,  { "SSAO Falloff", "Distance at which ambient occlusion fades out." } }
};

CONFIG_DEFINE_LOCALE(FilmGrain)
{
    { ELanguage::English,  { "Film Grain", "Add cinematic film grain noise to the image." } },
    { ELanguage::Japanese, { "フィルムグレイン", "映画風のノイズを追加します。" } },
    { ELanguage::German,   { "Filmkörnung", "Kinematisches Filmkorn zum Bild hinzufügen." } },
    { ELanguage::French,   { "Grain de Film", "Ajouter du grain cinématographique à l'image." } },
    { ELanguage::Spanish,  { "Grano de Película", "Añadir grano cinematográfico a la imagen." } },
    { ELanguage::Italian,  { "Grana Pellicola", "Aggiungi grana cinematografica all'immagine." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EFilmGrain)
{
    {
        ELanguage::English,
        {
            { EFilmGrain::Off,    { "Off", "" } },
            { EFilmGrain::Light,  { "Light", "" } },
            { EFilmGrain::Medium, { "Medium", "" } },
            { EFilmGrain::Heavy,  { "Heavy", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(FilmGrainIntensity)
{
    { ELanguage::English,  { "Grain Intensity", "Intensity of the film grain effect (0.0-1.0)." } },
    { ELanguage::Japanese, { "グレイン強度", "フィルムグレインの強度" } },
    { ELanguage::German,   { "Körnung Intensität", "Intensität der Filmkörnung." } },
    { ELanguage::French,   { "Intensité du Grain", "Intensité de l'effet de grain." } },
    { ELanguage::Spanish,  { "Intensidad del Grano", "Intensidad del efecto de grano." } },
    { ELanguage::Italian,  { "Intensità Grana", "Intensità dell'effetto grana." } }
};

CONFIG_DEFINE_LOCALE(ChromaticAberration)
{
    { ELanguage::English,  { "Chromatic Aberration", "Simulate lens color fringing effect." } },
    { ELanguage::Japanese, { "色収差", "レンズの色ずれ効果をシミュレートします。" } },
    { ELanguage::German,   { "Chromatische Aberration", "Linsen-Farbsaum-Effekt simulieren." } },
    { ELanguage::French,   { "Aberration Chromatique", "Simuler l'effet de frange colorée." } },
    { ELanguage::Spanish,  { "Aberración Cromática", "Simular efecto de franja de color." } },
    { ELanguage::Italian,  { "Aberrazione Cromatica", "Simula l'effetto di frangia colorata." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EChromaticAberration)
{
    {
        ELanguage::English,
        {
            { EChromaticAberration::Off,    { "Off", "" } },
            { EChromaticAberration::Subtle, { "Subtle", "" } },
            { EChromaticAberration::Normal, { "Normal", "" } },
            { EChromaticAberration::Strong, { "Strong", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(ChromaticAberrationIntensity)
{
    { ELanguage::English,  { "CA Intensity", "Intensity of the chromatic aberration effect." } },
    { ELanguage::Japanese, { "色収差強度", "色収差効果の強度" } },
    { ELanguage::German,   { "CA Intensität", "Intensität der chromatischen Aberration." } },
    { ELanguage::French,   { "Intensité CA", "Intensité de l'aberration chromatique." } },
    { ELanguage::Spanish,  { "Intensidad CA", "Intensidad de la aberración cromática." } },
    { ELanguage::Italian,  { "Intensità AC", "Intensità dell'aberrazione cromatica." } }
};

CONFIG_DEFINE_LOCALE(Upscaler)
{
    { ELanguage::English,  { "Upscaler", "AI/algorithmic upscaling technology (FSR 3, DLSS, XeSS)." } },
    { ELanguage::Japanese, { "アップスケーラー", "AI/アルゴリズムアップスケーリング技術" } },
    { ELanguage::German,   { "Upscaler", "KI/algorithmische Upscaling-Technologie." } },
    { ELanguage::French,   { "Upscaler", "Technologie d'upscaling IA/algorithmique." } },
    { ELanguage::Spanish,  { "Upscaler", "Tecnología de escalado IA/algorítmica." } },
    { ELanguage::Italian,  { "Upscaler", "Tecnologia di upscaling IA/algoritmica." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EUpscaler)
{
    {
        ELanguage::English,
        {
            { EUpscaler::Off,     { "Off", "" } },
            { EUpscaler::FSR1,    { "FSR 1.0", "" } },
            { EUpscaler::FSR3,    { "FSR 3.0", "" } },
            { EUpscaler::DLSS,    { "DLSS", "" } },
            { EUpscaler::XeSS,    { "XeSS", "" } },
            { EUpscaler::MetalFX, { "MetalFX", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(UpscaleQuality)
{
    { ELanguage::English,  { "Upscale Quality", "Quality preset for upscaling (affects render resolution)." } },
    { ELanguage::Japanese, { "アップスケール品質", "アップスケーリングの品質プリセット" } },
    { ELanguage::German,   { "Upscale-Qualität", "Qualitätsvoreinstellung für Upscaling." } },
    { ELanguage::French,   { "Qualité Upscale", "Préréglage de qualité d'upscaling." } },
    { ELanguage::Spanish,  { "Calidad Upscale", "Preajuste de calidad de escalado." } },
    { ELanguage::Italian,  { "Qualità Upscale", "Preimpostazione qualità upscaling." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EUpscaleQuality)
{
    {
        ELanguage::English,
        {
            { EUpscaleQuality::UltraQuality,     { "Ultra Quality", "" } },
            { EUpscaleQuality::Quality,          { "Quality", "" } },
            { EUpscaleQuality::Balanced,         { "Balanced", "" } },
            { EUpscaleQuality::Performance,      { "Performance", "" } },
            { EUpscaleQuality::UltraPerformance, { "Ultra Performance", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(FrameGeneration)
{
    { ELanguage::English,  { "Frame Generation", "Generate interpolated frames for higher FPS (requires FSR 3 or DLSS 3)." } },
    { ELanguage::Japanese, { "フレーム生成", "フレームを補間して高FPSを実現（FSR 3またはDLSS 3が必要）" } },
    { ELanguage::German,   { "Frame-Generierung", "Generiert interpolierte Frames für höhere FPS." } },
    { ELanguage::French,   { "Génération de Frames", "Génère des frames interpolées pour un FPS plus élevé." } },
    { ELanguage::Spanish,  { "Generación de Frames", "Genera frames interpolados para mayor FPS." } },
    { ELanguage::Italian,  { "Generazione Frame", "Genera frame interpolati per FPS più alti." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EFrameGeneration)
{
    {
        ELanguage::English,
        {
            { EFrameGeneration::Off,    { "Off", "" } },
            { EFrameGeneration::FSR3FG, { "FSR 3 Frame Gen", "" } },
            { EFrameGeneration::DLSSFG, { "DLSS Frame Gen", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(UpscaleSharpness)
{
    { ELanguage::English,  { "Upscale Sharpness", "Sharpening intensity for upscaled image (0.0-1.0)." } },
    { ELanguage::Japanese, { "アップスケールシャープネス", "アップスケール後のシャープネス強度" } },
    { ELanguage::German,   { "Upscale-Schärfe", "Schärfungsintensität für hochskaliertes Bild." } },
    { ELanguage::French,   { "Netteté Upscale", "Intensité de netteté pour l'image mise à l'échelle." } },
    { ELanguage::Spanish,  { "Nitidez Upscale", "Intensidad de nitidez para imagen escalada." } },
    { ELanguage::Italian,  { "Nitidezza Upscale", "Intensità nitidezza per immagine upscalata." } }
};

CONFIG_DEFINE_LOCALE(AntiAliasing)
{
    { ELanguage::English,  { "Anti-Aliasing", "Adjust the amount of smoothing applied to jagged edges." } },
    { ELanguage::Japanese, { "アンチエイリアス", "アンチエイリアスの種類を選択できます" } },
    { ELanguage::German,   { "Kantenglättung", "Passe die Menge an Kantenglättung an." } },
    { ELanguage::French,   { "Anticrénelage", "Ajuste le niveau d'anticrénelage appliqué aux bords des objets." } },
    { ELanguage::Spanish,  { "Anti-Aliasing", "Ajusta el nivel de suavizado aplicado a los dientes de sierra." } },
    { ELanguage::Italian,  { "Anti-Aliasing", "Regola la quantità di smussamento applicata ai bordi." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EAntiAliasing)
{
    {
        ELanguage::English,
        {
            { EAntiAliasing::Off, { "Off", "" } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EAntiAliasing::Off, { "オフ", "" } },
        }
    },
    {
        ELanguage::German,
        {
            { EAntiAliasing::Off, { "Aus", "" } }
        }
    },
    {
        ELanguage::French,
        {
            { EAntiAliasing::Off, { "Non", "" } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { EAntiAliasing::Off, { "No", "" } },
        }
    },
    {
        ELanguage::Italian,
        {
            { EAntiAliasing::Off, { "No", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(TransparencyAntiAliasing)
{
    { ELanguage::English,  { "Transparency Anti-Aliasing", "Apply anti-aliasing to alpha transparent textures." } },
    { ELanguage::Japanese, { "透明度のアンチエイリアス", "透過テクスチャにアンチエイリアスを適用します" } },
    { ELanguage::German,   { "Transparenz-Kantenglättung", "Wende Kantenglättung auf Alpha-Transparenz-Texturen an." } },
    { ELanguage::French,   { "Anticrénelage de transparence", "Applique l'anticrénelage sur les textures transparentes." } },
    { ELanguage::Spanish,  { "Anti-Aliasing de transparencias", "Aplica antialiasing a las texturas transparentes." } },
    { ELanguage::Italian,  { "Anti-Aliasing su texture trasparenti", "Applica l'anti-aliasing alle texture trasparenti." } }
};

CONFIG_DEFINE_LOCALE(ShadowResolution)
{
    { ELanguage::English,  { "Shadow Resolution", "Set the resolution of real-time shadows." } },
    { ELanguage::Japanese, { "影の解像度", "影の解像度を設定できます" } },
    { ELanguage::German,   { "Schattenauflösung", "Stelle die Auflösung der Echtzeit-Schatten ein." } },
    { ELanguage::French,   { "Résolution des ombres", "Définit la résolution des ombres en temps réel." } },
    { ELanguage::Spanish,  { "Resolución de sombras", "Establece la resolución de las sombras en tiempo real." } },
    { ELanguage::Italian,  { "Risoluzione ombre", "Imposta la risoluzioni delle ombre in tempo reale." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EShadowResolution)
{
    {
        ELanguage::English,
        {
            { EShadowResolution::Original, { "Original", "" } },
            { EShadowResolution::x512,     { "512x512", "" } },
            { EShadowResolution::x1024,    { "1024x1024", "" } },
            { EShadowResolution::x2048,    { "2048x2048", "" } },
            { EShadowResolution::x4096,    { "4096x4096", "" } },
            { EShadowResolution::x8192,    { "8192x8192", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(ShadowFilter)
{
    { ELanguage::English,  { "Shadow Filter", "Shadow filtering quality (PCF = softer edges, PCSS = contact hardening)." } },
    { ELanguage::Japanese, { "シャドウフィルター", "影のフィルタリング品質" } },
    { ELanguage::German,   { "Schatten-Filter", "Schattenfilterqualität." } },
    { ELanguage::French,   { "Filtre d'ombre", "Qualité de filtrage des ombres." } },
    { ELanguage::Spanish,  { "Filtro de sombras", "Calidad del filtrado de sombras." } },
    { ELanguage::Italian,  { "Filtro ombre", "Qualità del filtraggio delle ombre." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EShadowFilter)
{
    {
        ELanguage::English,
        {
            { EShadowFilter::Off,    { "Off (Hard)", "" } },
            { EShadowFilter::PCF3x3, { "PCF 3x3", "" } },
            { EShadowFilter::PCF5x5, { "PCF 5x5", "" } },
            { EShadowFilter::PCF7x7, { "PCF 7x7", "" } },
            { EShadowFilter::PCSS,   { "PCSS (Soft)", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(ReflectionResolution)
{
    { ELanguage::English,  { "Reflection Resolution", "Set the resolution of real-time reflections." } },
    { ELanguage::Japanese, { "反射解像度", "リアルタイム反射の解像度を設定します" } },
    { ELanguage::German,   { "Reflektionsauflösung", "Bestimmt die Auflösung der Echtzeitreflektionen." } },
    { ELanguage::French,   { "Résolution des reflets", "Définir la résolution des reflets en temps réel." } },
    { ELanguage::Spanish,  { "Resolución de reflejos", "Establece la resolución de los reflejos en tiempo real." } },
    { ELanguage::Italian,  { "Risoluzione riflessi", "Imposta la risoluzione dei riflessi in tempo reale." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EReflectionResolution)
{
    {
        ELanguage::English,
        {
            { EReflectionResolution::Eighth, { "12.5%", "" } },
            { EReflectionResolution::Quarter, { "25%", "" } },
            { EReflectionResolution::Half, { "50%", "" } },
            { EReflectionResolution::Full, { "100%", "" } }
        }
    }
};

CONFIG_DEFINE_LOCALE(RadialBlur)
{
    { ELanguage::English,  { "Radial Blur", "Change the quality of the radial blur." } },
    { ELanguage::Japanese, { "放射状ぼかし", "放射状ぼかしの品質を変更します" } },
    { ELanguage::German,   { "Radiale Unschärfe", "Verändere die Qualität der radialen Unschärfe." } },
    { ELanguage::French,   { "Flou Radial", "Modifier la qualité du flou radial." } },
    { ELanguage::Spanish,  { "Desenfoque radial", "Cambia la calidad del desenfoque radial." } },
    { ELanguage::Italian,  { "Sfocatura radiale", "Modifica la qualità della sfocatura radiale." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ERadialBlur)
{
    {
        ELanguage::English,
        {
            { ERadialBlur::Off,      { "Off", "" } },
            { ERadialBlur::Original, { "Original", "" } },
            { ERadialBlur::Enhanced, { "Enhanced", "Enhanced: uses more samples for smoother radial blur." } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { ERadialBlur::Off,      { "オフ", "" } },
            { ERadialBlur::Original, { "オリジナル", "" } },
            { ERadialBlur::Enhanced, { "エンハンスド", "エンハンスド：より多くのサンプルを使用してよりスムーズな放射状ぼかしを実現します" } }
        }
    },
    {
        ELanguage::German,
        {
            { ERadialBlur::Off,      { "Aus", "" } },
            { ERadialBlur::Original, { "Original", "" } },
            { ERadialBlur::Enhanced, { "Verbessert", "Verbessert: Benutzt mehr Samples um eine weichere radiale Unschärfe zu erzeugen." } }
        }
    },
    {
        ELanguage::French,
        {
            { ERadialBlur::Off,      { "Non", "" } },
            { ERadialBlur::Original, { "Original", "" } },
            { ERadialBlur::Enhanced, { "Amélioré", "Amélioré : utilise plus d'échantillons pour un flou radial plus lisse." } }
        }
    },
    {
        ELanguage::Spanish,
        {
            { ERadialBlur::Off,      { "No", "" } },
            { ERadialBlur::Original, { "Original", "" } },
            { ERadialBlur::Enhanced, { "Mejorado", "Mejorado: utiliza más muestras para un desenfoque radial más suave." } }
        }
    },
    {
        ELanguage::Italian,
        {
            { ERadialBlur::Off,      { "No", "" } },
            { ERadialBlur::Original, { "Originale", "" } },
            { ERadialBlur::Enhanced, { "Migliorato", "Migliorato: utilizza più campioni per una sfocatura radiale più uniforme." } }
        }
    }
};

CONFIG_DEFINE_LOCALE(CutsceneAspectRatio)
{
    { ELanguage::English,  { "Cutscene Aspect Ratio", "Change the aspect ratio of the real-time cutscenes." } },
    { ELanguage::Japanese, { "アスペクト比のカットシーン", "リアルタイムカットシーンのアスペクト比を変更できます" } },
    { ELanguage::German,   { "Zwischensequenz-Seitenverhältnis", "Verändere das Seitenverhältnis der Echtzeit-Zwischensequenzen." } },
    { ELanguage::French,   { "Format des cinématiques", "Modifie le format d'image des cinématiques en temps réel." } },
    { ELanguage::Spanish,  { "Relación de aspecto de cinemáticas", "Cambia la relación de aspecto de las cinemáticas en tiempo real." } },
    { ELanguage::Italian,  { "Rapporto d'aspetto dei filmati", "Cambia il rapporto d'aspetto dei filmati in tempo reale." } }
};

CONFIG_DEFINE_ENUM_LOCALE(ECutsceneAspectRatio)
{
    {
        ELanguage::English,
        {
            { ECutsceneAspectRatio::Original, { "Original", "Original: locks cutscenes to their original 16:9 aspect ratio." } },
            { ECutsceneAspectRatio::Unlocked, { "Unlocked", "Unlocked: allows cutscenes to adjust their aspect ratio to the window size. WARNING: this will introduce visual oddities past the original 16:9 aspect ratio." } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { ECutsceneAspectRatio::Original, { "オリジナル", "オリジナル: カットシーンを元の16:9のアスペクト比に固定します" } },
            { ECutsceneAspectRatio::Unlocked, { "解除", "解除: カットシーンのアスペクト比をウィンドウサイズに合わせて調整します 警告: 元の16:9のアスペクト比を超えると視覚的な異常が発生します" } },
        }
    },
    {
        ELanguage::German,
        {
            { ECutsceneAspectRatio::Original, { "Original", "Original: Behält die Zwischensequenzen in ihrem originalen 16:9 Seitenverhältnis." } },
            { ECutsceneAspectRatio::Unlocked, { "Entsperrt", "Entsperrt: Erlaubt Zwischensequenzen ihr Seitenverhältnis der Fenstergröße anzupassen. WARNUNG: Diese Option kann zu visuellen Fehlern außerhalb der Grenzen des ursprünglichen Seitenverhältnisses führen." } }
        }
    },
    {
        ELanguage::French,
        {
            { ECutsceneAspectRatio::Original, { "Original", "Original : force les cinématiques dans leur format 16:9 d'origine." } },
            { ECutsceneAspectRatio::Unlocked, { "Libre", "Libre : permet aux cinématiques d'adapter leur format d'image à la taille de la fenêtre. ATTENTION : au delà du format 16:9 d'origine, des bugs visuels apparaitront." } },
        }
    },
    {
        ELanguage::Spanish,
        {
            { ECutsceneAspectRatio::Original, { "Original", "Original: muestra las cinemáticas en su relación de aspecto original de 16:9." } },
            { ECutsceneAspectRatio::Unlocked, { "Desbloqueado", "Desbloqueado: permite que las cinemáticas ajusten su relación de aspecto al tamaño de la ventana. ADVERTENCIA: esto introducirá rarezas visuales más allá de la relación de aspecto original de 16:9." } },
        }
    },
    {
        ELanguage::Italian,
        {
            { ECutsceneAspectRatio::Original, { "Originale", "Originale: blocca il rapporto d'aspetto dei filmati a 16:9." } },
            { ECutsceneAspectRatio::Unlocked, { "Sbloccato", "Sbloccato: il rapporto d'aspetto verrà regolato in base alle dimensioni della finestra. ATTENZIONE: questa opzione potrebbe causare dei problemi visivi se il rapporto d'aspetto è oltre 16:9." } }
        }
    }
};

CONFIG_DEFINE_LOCALE(UIAlignmentMode)
{
    { ELanguage::English,  { "UI Alignment Mode", "Change how the UI aligns with the display." } },
    { ELanguage::Japanese, { "UIアライメントモード", "UIとディスプレイの配置を変更できます" } },
    { ELanguage::German,   { "Benutzeroberflächenausrichtung", "Verändere wie die Benutzeroberfläche sich mit dem Bildschirm ausrichtet." } },
    { ELanguage::French,   { "Alignement de l'IU", "Modifie l'alignement de l'interface utilisateur sur l'écran." } },
    { ELanguage::Spanish,  { "Modo de alineamiento de UI", "Cambia la alineación de la interfaz de usuario con la pantalla." } },
    { ELanguage::Italian,  { "Modalità allineamento interfaccia", "Modifica come l'interfaccia si allinea con lo schermo." } }
};

CONFIG_DEFINE_ENUM_LOCALE(EUIAlignmentMode)
{
    {
        ELanguage::English,
        {
            { EUIAlignmentMode::Edge,    { "Edge", "Edge: the UI will align with the edges of the display." } },
            { EUIAlignmentMode::Centre,  { "Center", "Center: the UI will align with the center of the display." } }
        }
    },
    {
        ELanguage::Japanese,
        {
            { EUIAlignmentMode::Edge,    { "エッジ", "エッジ: UIがディスプレイの端に揃います" } },
            { EUIAlignmentMode::Centre,  { "センター", "センター: UIがディスプレイの中央に揃います" } },
        }
    },
    {
        ELanguage::German,
        {
            { EUIAlignmentMode::Edge,    { "Rand", "Rand: Die Benutzeroberfläche richtet sich zum Rand des Bildschirms aus." } },
            { EUIAlignmentMode::Centre,  { "Mitte", "Mitte: Die Benutzeroberfläche richtet sich zur Mitte des Bildschirms aus." } }
        }
    },
    {
        ELanguage::French,
        {
            { EUIAlignmentMode::Edge,    { "Bord", "Bord : l'interface utilisateur sera alignée sur les bords de l'écran." } },
            { EUIAlignmentMode::Centre,  { "Centrée", "Centrée : l'interface utilisateur sera alignée sur le centre de l'écran." } },
        }
    },
    {
        ELanguage::Spanish,
        {
            { EUIAlignmentMode::Edge,    { "Borde", "Borde: la interfaz de usuario se alineará con los bordes de la pantalla." } },
            { EUIAlignmentMode::Centre,  { "Centro", "Centro: la interfaz de usuario se alineará con el centro de la pantalla." } },
        }
    },
    {
        ELanguage::Italian,
        {
            { EUIAlignmentMode::Edge,    { "Bordi", "Bordi: l'interfaccia si allineerà con i bordi dello schermo." } },
            { EUIAlignmentMode::Centre,  { "Centro", "Centro: l'interfaccia si allineerà con il centro dello schermo." } }
        }
    }
};

// Post-Processing Effect Locales
CONFIG_DEFINE_LOCALE(VignetteEnabled)
{
    { ELanguage::English,  { "Vignette", "Enable screen edge darkening vignette effect." } },
    { ELanguage::Japanese, { "ビネット", "画面端を暗くするビネット効果を有効にします" } },
    { ELanguage::German,   { "Vignette", "Bildschirmrand-Verdunkelungseffekt aktivieren." } },
    { ELanguage::French,   { "Vignettage", "Activer l'effet d'assombrissement des bords." } },
    { ELanguage::Spanish,  { "Viñeta", "Habilitar efecto de oscurecimiento de bordes." } },
    { ELanguage::Italian,  { "Vignettatura", "Abilita effetto di oscuramento dei bordi." } }
};

CONFIG_DEFINE_LOCALE(VignetteIntensity)
{
    { ELanguage::English,  { "Vignette Intensity", "Strength of the vignette darkening effect (0-1)." } },
    { ELanguage::Japanese, { "ビネット強度", "ビネット効果の強さ（0〜1）" } },
    { ELanguage::German,   { "Vignette Intensität", "Stärke des Vignetten-Effekts (0-1)." } },
    { ELanguage::French,   { "Intensité Vignette", "Force de l'effet de vignettage (0-1)." } },
    { ELanguage::Spanish,  { "Intensidad Viñeta", "Fuerza del efecto de viñeta (0-1)." } },
    { ELanguage::Italian,  { "Intensità Vignettatura", "Forza dell'effetto vignettatura (0-1)." } }
};

CONFIG_DEFINE_LOCALE(VignetteRadius)
{
    { ELanguage::English,  { "Vignette Radius", "Inner radius where vignette effect begins (0-1)." } },
    { ELanguage::Japanese, { "ビネット半径", "ビネット効果が始まる内側の半径（0〜1）" } },
    { ELanguage::German,   { "Vignette Radius", "Innerer Radius wo der Vignetten-Effekt beginnt (0-1)." } },
    { ELanguage::French,   { "Rayon Vignette", "Rayon intérieur où l'effet commence (0-1)." } },
    { ELanguage::Spanish,  { "Radio Viñeta", "Radio interior donde comienza el efecto (0-1)." } },
    { ELanguage::Italian,  { "Raggio Vignettatura", "Raggio interno dove inizia l'effetto (0-1)." } }
};

CONFIG_DEFINE_LOCALE(VignetteSoftness)
{
    { ELanguage::English,  { "Vignette Softness", "Falloff softness of the vignette edge (higher = softer)." } },
    { ELanguage::Japanese, { "ビネットソフトネス", "ビネットエッジの減衰の柔らかさ" } },
    { ELanguage::German,   { "Vignette Weichheit", "Weichheit des Vignetten-Rands." } },
    { ELanguage::French,   { "Douceur Vignette", "Douceur du dégradé du vignettage." } },
    { ELanguage::Spanish,  { "Suavidad Viñeta", "Suavidad del borde de la viñeta." } },
    { ELanguage::Italian,  { "Morbidezza Vignettatura", "Morbidezza del bordo della vignettatura." } }
};

CONFIG_DEFINE_LOCALE(VignetteRoundness)
{
    { ELanguage::English,  { "Vignette Roundness", "Shape of vignette (0 = follows aspect ratio, 1 = circular)." } },
    { ELanguage::Japanese, { "ビネット丸み", "ビネットの形状（0=アスペクト比に従う、1=円形）" } },
    { ELanguage::German,   { "Vignette Rundheit", "Form der Vignette (0 = Seitenverhältnis, 1 = Kreisförmig)." } },
    { ELanguage::French,   { "Rondeur Vignette", "Forme du vignettage (0 = ratio, 1 = circulaire)." } },
    { ELanguage::Spanish,  { "Redondez Viñeta", "Forma de la viñeta (0 = ratio, 1 = circular)." } },
    { ELanguage::Italian,  { "Rotondità Vignettatura", "Forma della vignettatura (0 = ratio, 1 = circolare)." } }
};

CONFIG_DEFINE_LOCALE(TAABlendFactor)
{
    { ELanguage::English,  { "TAA Blend Factor", "TAA temporal blending strength (0.05-0.2 typical)." } },
    { ELanguage::Japanese, { "TAAブレンド係数", "TAAの時間的ブレンドの強さ" } },
    { ELanguage::German,   { "TAA Mischfaktor", "TAA temporale Mischstärke." } },
    { ELanguage::French,   { "Facteur TAA", "Force du mélange temporel TAA." } },
    { ELanguage::Spanish,  { "Factor TAA", "Fuerza de mezcla temporal TAA." } },
    { ELanguage::Italian,  { "Fattore TAA", "Forza miscelazione temporale TAA." } }
};

CONFIG_DEFINE_LOCALE(TAAMotionScale)
{
    { ELanguage::English,  { "TAA Motion Scale", "Scale factor for motion vector influence in TAA." } },
    { ELanguage::Japanese, { "TAAモーションスケール", "TAAでのモーションベクトルの影響度" } },
    { ELanguage::German,   { "TAA Bewegungsskala", "Skalierungsfaktor für Bewegungsvektoren." } },
    { ELanguage::French,   { "Échelle Mouvement TAA", "Facteur d'échelle pour les vecteurs de mouvement." } },
    { ELanguage::Spanish,  { "Escala Movimiento TAA", "Factor de escala para vectores de movimiento." } },
    { ELanguage::Italian,  { "Scala Movimento TAA", "Fattore di scala per vettori di movimento." } }
};

CONFIG_DEFINE_LOCALE(SMAAEdgeThreshold)
{
    { ELanguage::English,  { "SMAA Edge Threshold", "Edge detection sensitivity for SMAA (lower = more edges)." } },
    { ELanguage::Japanese, { "SMAAエッジ閾値", "SMAAのエッジ検出感度" } },
    { ELanguage::German,   { "SMAA Kantenschwelle", "Kantenerkennungsempfindlichkeit für SMAA." } },
    { ELanguage::French,   { "Seuil Bords SMAA", "Sensibilité de détection des bords SMAA." } },
    { ELanguage::Spanish,  { "Umbral Bordes SMAA", "Sensibilidad de detección de bordes SMAA." } },
    { ELanguage::Italian,  { "Soglia Bordi SMAA", "Sensibilità rilevamento bordi SMAA." } }
};

CONFIG_DEFINE_LOCALE(FSR1Sharpness)
{
    { ELanguage::English,  { "FSR 1.0 Sharpness", "Sharpening strength for FSR 1.0 RCAS pass (0-1)." } },
    { ELanguage::Japanese, { "FSR 1.0シャープネス", "FSR 1.0 RCASパスのシャープ化の強さ" } },
    { ELanguage::German,   { "FSR 1.0 Schärfe", "Schärfestärke für FSR 1.0 RCAS." } },
    { ELanguage::French,   { "Netteté FSR 1.0", "Force de netteté pour FSR 1.0 RCAS." } },
    { ELanguage::Spanish,  { "Nitidez FSR 1.0", "Fuerza de nitidez para FSR 1.0 RCAS." } },
    { ELanguage::Italian,  { "Nitidezza FSR 1.0", "Forza nitidezza per FSR 1.0 RCAS." } }
};
