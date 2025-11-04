#pragma once

enum class TypeEffet {
      // Effets basiques
      AJOUT_OR,
      AJOUT_COMBAT,
      AJOUT_VIE,

      // Effets cartes
      PIOCHER,
      DEFAUSSER, //pour l'adversaire
      SACRIFIER,

      // EFfets champions
      ASSOMMER_CHAMPION,
      PREPARE_CHAMPION,

      // Effets "or"
      COMBAT_OU_VIE,
      OR_OU_COMBAT,

      // Effets "per"
      COMBAT_PAR_CHAMPION,
      COMBAT_PAR_GARDE,
      COMBAT_PAR_ALLIE,
      VIE_PAR_CHAMPION,

      // Effets "or" et "per"
      OR_OU_VIE_PAR_CHAMPION,

      // Effets "defausse au dessus du deck"
      DEFAUSSE_AU_DESSUS_DU_DECK,
      CHAMPION_DEFAUSSE_AU_DESSUS_DECK,

      // Effets "prochain achat"
      PROCHAIN_ACHAT_ACTION_DESSUS_DECK,
      PROCHAIN_ACHAT_DANS_MAIN,

      // Effets "si oui"
      SACRIFIER_SIOUI_AJOUT_COMBAT,
      PIOCHE_SIOUI_DEFAUSSE,

      };
