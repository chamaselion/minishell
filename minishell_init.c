/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2025/01/20 16:08:35 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

const char	**order_init(void)
{
    static const char *events[256] = {
        "The battlefield falls silent as Jedi Master Tiplar turns... only to be struck down by Tup. (Ringo Vinda)",
        "A piercing clang echoes as Maul wields twisted metal. Ridge's end is swift and merciless. *A villain falls.*",
        "R7-A7 beeps one last warning... but the clone's blaster fires. Sparks fly as it crumbles. (Later Rebuilt)",
        "CH-33P \"Cheep\" and RG-G1 \"G-G\" try to flee, but relentless fire from Jesse's troops ends them.",
        "*Jesse meets his fate in the Star Destroyer crash. Redemption comes too late.*",
        "High in the mountains of Utapau, loyal Boga cries out. A blast shatters the cliffside; she plummets to her doom.",
        "On Mygeeto, Jedi Master Ki-Adi-Mundi rallies his troops, unaware of Bacara’s betrayal. He falls in battle.",
        "Aayla Secura strides confidently on Felucia, but Bly’s raised blaster ensures her tragic end.",
        "Plo Koon maneuvers his starfighter. Jag’s precision shot ignites the skies. Koon falls in flames.",
        "Speeding through Saleucami, Stass Allie looks back in shock as Neyo fires. The speeder crumples.",
        "*Clone Captain \"Jek\" lunges at Yoda but meets the Grandmaster’s saber. A traitor falls.*",
        "*Commander \"Gree\" learns not to challenge Yoda. His severed head hits the forest floor.*",
        "Zett Jukassa defies the odds, lightsaber blazing, but Fox’s fire ends the Padawan’s stand.",
        "Shaak Ti meditates in the Jedi Temple, unaware of Vader’s saber. Her end comes silently.",
        "Whie Malreaux fights bravely, but Vader’s saber ensures his fall.",
        "Bene claws at her throat, Vader’s dark power squeezing the life from her.",
        "Cin Drallig, the Swordmaster, puts up a valiant fight, but Vader’s blade triumphs.",
        "Depa Billaba fights on Kaller, protecting her Padawan. Grey’s betrayal ensures her sacrifice.",
        "*Commander Grey redeems himself in a blaze of glory, sacrificing all to destroy a villain.*",
        "From Felucia to Mygeeto, lightsabers extinguish. Order 66 claims them all. The galaxy weeps."
    };
	return (events);
}
void dramaticPause(int milliseconds)
{
    usleep(milliseconds * 1000);
}

void printBold(const char *text)
{
    printf("\033[1m%s\033[0m", text);
}

void printEvent(const char *events[256], int index, int isBold)
{
    if (isBold)
        printBold(events[index]);
    else 
        printf("%s", events[index]);
    printf("\n");
    dramaticPause(2000);
}

void order_extra(void)
{
	int i;
	const char **events;

	events = order_init();

	i = 0;
	printf("\n======= EXECUTING ORDER 66 =======\n");
    dramaticPause(3000);
    while (i < 20)
	{
        int isBold = (events[i][0] == '*');
        if (isBold)
            printEvent(events, i, 1);
		else
            printEvent(events, i, 0);
		i++;
    }
    printf("\n======= ORDER 66 EXECUTED =======\n");
}

void	check_order(t_token *tokens)
{
	t_token *ct;

	ct = tokens;
	while (ct)
	{
		if (strcmp(ct->content, "order_66") == 0)
		{
			order_extra();
			exit(0);
		}
		ct = ct->next;
	}
	
}

void init_shell(t_shell *shell, t_env_var *env_vars) 
{
    shell->commands = NULL;
	shell->env_vars = env_vars;
    getcwd(shell->cwd, sizeof(shell->cwd));
}

void init_token(t_token *token, t_raw_token *t_raw_token)
{
    token->next = NULL;
    token->prev = NULL;
    token->role = ROLE_DEFAULT;
    token->command_expected = 0;
    token->quote_state = t_raw_token->quote_state;
}
