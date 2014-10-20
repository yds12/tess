#include "lang.h"

using namespace Tess;

const char* Lang::get(int code)
{
	switch(Support::settings.language)
	{
		case LANG_PORTUGUESE:
			
			switch(code)
			{
				case 0: return "jogo individual";
				case 1: return "dois jogadores";
				case 2: return "opções";
				case 3: return "ranking";
				case 4: return "créditos";
				case 5: return "sair";
				case 6: return "fácil";
				case 7: return "moderado";
				case 8: return "difícil";
				case 9: return "voltar";
				case 10: return "Jogo Individual";
				case 11: return "digite seu nome e tecle <enter> ou clique";
				case 12: return "vs computador";
				case 13: return "criar jogo";
				case 14: return "entrar no jogo";
				case 15: return "Jogo de Dois Jogadores";
				case 16: return "procurando servidores...";
				case 17: return "esperando jogadores se conectarem...";
				case 18: return "cancelar";			
				case 19: return "tela";
				case 20: return "música";
				case 21: return "som";
				case 22: return "idioma";
				case 23: return "tema";
				case 24: return "salvar e sair";
				case 25: return "janela";
				case 26: return "tela cheia";
				case 27: return "sim";
				case 28: return "não";
				case 29: return "redondo";
				case 30: return "redondo/negro";
				case 31: return "cores";
				case 32: return "negro";
				case 33: return "jóias";
				case 34: return "Opções";
				case 35: return "apresenta...";
				case 36: return "nome do jogador";
				case 37: return "pontos";
				case 38: return "linhas";	
				case 39: return "limpar rankings";
				case 40: return "não";
				case 41: return "sim";
				case 42: return "Rankings zerados";
				case 43: return "Todos os rankings foram zerados. Pressione <enter> para voltar";
				case 44: return "Ranking";
				case 45: return "Tem certeza que deseja zerar todos os rankings?";
				case 46: return "Ranking - Fácil";
				case 47: return "Ranking - Moderado";
				case 48: return "Ranking - Difícil";			
				case 49: return "próxima";
				case 50: return "PAUSADO";
				case 51: return "FIM DE JOGO";
				case 52: return "VOCÊ VENCEU";
				case 53: return "VOCÊ PERDEU";
				case 54: return "item";
				case 55: return "continuar jogo";
				case 56: return "voltar ao menu principal";
				case 57: return "Tem certeza que deseja abandonar o jogo?";
				case 58: return "plano de fundo";
				case 59: return "padrão";
				case 60: return "negro";
				case 61: return "feminino";
				default: return "";
			}			
			break;
			
		case LANG_SPANISH:
			
			switch(code)
			{
				case 0: return "juego individual";
				case 1: return "dos jugadores";
				case 2: return "opciones";
				case 3: return "ranking";
				case 4: return "créditos";
				case 5: return "salir";
				case 6: return "fácil";
				case 7: return "moderado";
				case 8: return "difícil";
				case 9: return "volver";
				case 10: return "Juego Individual";
				case 11: return "escriba su nombre y pulse <enter> o clic";
				case 12: return "vs computador";
				case 13: return "crear juego";
				case 14: return "entrar en el juego";
				case 15: return "Juego de Dos Jugadores";
				case 16: return "en busca de servidores...";
				case 17: return "esperando a los jugadores conectarse...";
				case 18: return "cancelar";			
				case 19: return "pantalla";
				case 20: return "música";
				case 21: return "sonido";
				case 22: return "idioma";
				case 23: return "tema";
				case 24: return "guardar y salir";
				case 25: return "ventana";
				case 26: return "pantalla completa";
				case 27: return "si";
				case 28: return "no";
				case 29: return "redondo";
				case 30: return "redondo/negro";
				case 31: return "colores";
				case 32: return "negro";
				case 33: return "joyas";
				case 34: return "Opciones";
				case 35: return "presenta...";
				case 36: return "nombre del jugador";
				case 37: return "puntos";
				case 38: return "líneas";	
				case 39: return "borrar rankings";
				case 40: return "no";
				case 41: return "si";
				case 42: return "Rankings borrados";
				case 43: return "Todos los rankings fueron borrados. Presione <enter> para volver";
				case 44: return "Ranking";
				case 45: return "¿Seguro de que deseas eliminar todos los rankings?";
				case 46: return "Ranking - Fácil";
				case 47: return "Ranking - Moderado";
				case 48: return "Ranking - Difícil";			
				case 49: return "próxima";
				case 50: return "EN PAUSA";
				case 51: return "FINAL DEL JUEGO";
				case 52: return "USTED GANÓ";
				case 53: return "USTED PERDIÓ";
				case 54: return "item";
				case 55: return "reanudar el juego";
				case 56: return "volver al menú principal";
				case 57: return "¿Está seguro que desea salir del juego?";
				case 58: return "fondo";
				case 59: return "estándar";
				case 60: return "negro";
				case 61: return "femenino";
				default: return "";
			}			
			break;
						
		default:
			
			switch(code)
			{
				case 0: return "single player";
				case 1: return "multiplayer";
				case 2: return "options";
				case 3: return "ranking";
				case 4: return "credits";
				case 5: return "quit";
				case 6: return "easy";
				case 7: return "moderate";
				case 8: return "hard";
				case 9: return "back";
				case 10: return "Single Player Game";
				case 11: return "type your name and press <enter> or click";
				case 12: return "vs computer";
				case 13: return "create game";
				case 14: return "join game";
				case 15: return "Multiplayer Game";
				case 16: return "searching for servers...";
				case 17: return "waiting for player to connect...";
				case 18: return "cancel";				
				case 19: return "screen";
				case 20: return "music";
				case 21: return "sound";
				case 22: return "language";
				case 23: return "skin";
				case 24: return "save and back";
				case 25: return "windowed";
				case 26: return "fullscreen";
				case 27: return "on";
				case 28: return "off";
				case 29: return "round";
				case 30: return "round/black";
				case 31: return "color";
				case 32: return "black";
				case 33: return "jewel";
				case 34: return "Options";
				case 35: return "presents...";
				case 36: return "player name";
				case 37: return "score";
				case 38: return "lines";				
				case 39: return "clear rankings";
				case 40: return "no";
				case 41: return "yes";
				case 42: return "Rankings cleared";
				case 43: return "All rankings were cleared. Press <enter> to go back";
				case 44: return "Ranking";
				case 45: return "Are you sure you want to clear all rankings?";
				case 46: return "Ranking - Easy";
				case 47: return "Ranking - Moderate";
				case 48: return "Ranking - Hard";				
				case 49: return "next";
				case 50: return "PAUSED";
				case 51: return "GAME OVER";
				case 52: return "YOU WIN";
				case 53: return "YOU LOSE";
				case 54: return "item";
				case 55: return "resume game";
				case 56: return "back to main menu";
				case 57: return "Are you sure you want to leave the game?";
				case 58: return "background";
				case 59: return "default";
				case 60: return "dark";
				case 61: return "female";
				default: return "";
			}
			break;
	}
}
