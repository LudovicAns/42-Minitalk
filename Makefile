# ----- Names ----- #
CLIENT	=	client
SERVER	=	server

# ----- Compiling Variables ----- #
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f

# ----- libft Variables ----- #
LIBFT_DIR	=	./42-Libft
LIBFT		=	$(LIBFT_DIR)/libft.a

# ----- Main Variables ----- #
SRC_CLIENT	=	client.c
SRC_SERVER	=	server.C
INC			=	-I. -I$(LIBFT_DIR)

# ----- Colors ----- #
GREEN		=	\e[38;5;118m
YELLOW		=	\e[38;5;226m
RESET		=	\e[0m
_SUCCESS	=	[$(GREEN)SUCCESS$(RESET)]
_INFO		=	[$(YELLOW)INFO$(RESET)]

all:

$(SERVER): $(LIBFT)
	@ $(CC) $(CFLAGS) $(SRC_SERVER) $(LIBFT) $(INC) -o $(SERVER)
	@ printf "$(_SUCCESS) 💻 server is now ready.\n"

$(CLIENT): $(LIBFT)
	@ $(CC) $(CFLAGS) $(SRC_CLIENT) $(LIBFT) $(INC) -o $(CLIENT)
	@ printf "$(_SUCCESS) 💂 server is now ready.\n"

$(LIBFT):
	@ printf "$(_INFO) 🔨 starting build libft.\n"
	@ $(MAKE) -C $(LIBFT_DIR)

clean:
	@ $(RM) $(CLIENT) $(SERVER)
	@ printf "$(_INFO) 🔥 client removed.\n"
	@ printf "$(_INFO) 🔥 server removed.\n"
	@ printf "$(_SUCCESS) ✅ repository succesfully cleaned.\n"

fclean: clean
	@ $(MAKE) fclean -C $(LIBFT_DIR)
	@ printf "$(_SUCCESS) ✅ repository succesfully fcleaned.\n"

re: fclean all

.PHONY: all clean fclean re