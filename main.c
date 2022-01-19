#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char file_path[10000];
char message[10000];
char endcode_file_path[] = "C:/ThienNQ/KTGT/endcode.txt";
int k;

void get_input_file_path()
{
	printf("Path to file: ");
	fflush(stdin);
	scanf("%s", file_path);
}
void get_input_message()
{
	printf("Message: ");
	fflush(stdin);
	fgets(message, sizeof(message)+1, stdin);
}
void get_input_k()
{
	printf("K: ");
	fflush(stdin);
	scanf("%d", &k);
}

char * endcode_message(int message_len)
{
	char *msg = malloc(message_len * 7);
	int index = 0;
	int i;
	for(i = 0; i < message_len; i++)
	{
		int j;
		for (j = 6; j >= 0; --j)
	    {
	        msg[index] = (message[i] & (1 << j)) ? '1' : '0';
	        index++;
	    }
	}
	return msg;
}

int characters_of_file()
{
	FILE *fp;
	fp = fopen(file_path, "r");
	int count = 0;
	char c;
	while(c = fgetc(fp)!= EOF) count++;
	fclose(fp);
	return count;
}

char * read_file_content(int num_char)
{
	FILE *fp;
	fp = fopen(file_path, "r");
	char *file_content = malloc(num_char);
	char c;
	int i = 0;
	rewind(fp);
    while ((c = fgetc(fp)) != EOF && i < num_char){
        file_content[i] = c;
        i++;
    }
	fclose(fp);
	return file_content;
}
int count_space(char *file_content, int num_char)
{
	int i, space_number = 0;
	for(i = 0; i < num_char; i++)
	{
		if(file_content[i] == 32)
			space_number++;
	}
	return space_number;
}
void write_to_file(char *content, int n)
{
	FILE *fp;
	fp = fopen(endcode_file_path, "w");
	int i;
	for(i = 0; i < n; i++)
	{
		fputc(content[i], fp);
	}
	fclose(fp);
}
void endcoding()
{
	get_input_file_path();
	get_input_message();
	get_input_k();
	
	int message_len = strlen(message) - 1;
	char msg[(message_len * 7) - 1];
	strncpy(msg, endcode_message(message_len), message_len*7);
	
	int num_char = characters_of_file();
	char file_content[num_char - 1];
	strncpy(file_content, read_file_content(num_char), num_char);
	
	int space_number = count_space(file_content, num_char);
	if((space_number / k) < (message_len * 7))
	{
		printf("File is to short");
		return;
	}
	
	int num_char_of_new_file = num_char + (message_len * 7);
	char endcode_content[num_char_of_new_file - 1];
	int i, j, h = 0;
	int space_flag = 0;
	while(i < num_char_of_new_file)
	{
		endcode_content[i] = file_content[j];
		if(file_content[j] == 32)
		{
			space_flag ++;
			if(msg[h] == '1' && space_flag == 1)
			{
				i++;
				endcode_content[i] = ' ';
				goto BREAK;
			}
			if(msg[h] == '0' && space_flag == 2)
			{
				i++;
				endcode_content[i] = ' ';
				goto BREAK;
			}
			
			BREAK: if(space_flag == k)
			{
				h++;
				space_flag = 0;
			}
		}
		i++;
		j++;
	}
	write_to_file(endcode_content, num_char_of_new_file);
}

void decode_machester(char *content, int len)
{
	int i = 0;
	int j = 0;
	int h = 0;
	int count = 0;
	char bin_char[7];
	while(i < (len - 1))
	{
		if(content[i]=='0' && content[i+1]=='1')
		{
			bin_char[j] = '0';
			j++;
			count++;
			if(count == 7)
			{
				message[h] = strtol(bin_char, 0, 2);
				h++;
				count = 0;
				j = 0;
			}
		}
		else if(content[i]=='1' && content[i+1]=='0')
		{
			bin_char[j] = '1';
			j++;
			count++;
			if(count == 7)
			{
				message[h] = strtol(bin_char, 0, 2);
				h++;
				count = 0;
				j = 0;
			}
		}
		else{
			break;
		}
		i += 2;
	}
	printf("%s", message);
}

void decoding()
{
	get_input_file_path();
	get_input_k();
	
	int num_char = characters_of_file();
	char file_content[num_char - 1];
	strncpy(file_content, read_file_content(num_char), num_char);
	int space_number = count_space(file_content, num_char);
	int i = 0;
	int num_bit = 0;
	char temp_msg[10000];
	while(i<num_char)
	{
		if(file_content[i] == 32)
		{
			if(file_content[i+1] != 32)
			{
				temp_msg[num_bit] = '0';
				num_bit++;
			}
			else
			{
				temp_msg[num_bit] = '1';
				num_bit++;
				i++;
			}
		}
		i++;
	}
	int flag = 0;
	char bin_split[num_bit];
	int bin_len = 0;
	i=0;
	while(i<num_bit)
	{
		flag++;
		if(flag == 1 || flag == 2)
		{
			bin_split[bin_len] = temp_msg[i];
			bin_len++;	
		}
		if(flag == k)
			flag = 0;
		i++;
	}
	decode_machester(bin_split, bin_len);
}

void menu()
{
	printf("1. Endcode message\n");
	printf("2. Decode message\n");
}

int main(int argc, char *argv[]) {
	menu();
	int choose;
	scanf("%d", &choose);
	switch(choose){
		case 1:{
			endcoding();
			break;
		}
		case 2:{
			decoding();
			break;
		}
		default:{
			printf("Choose error\n");
			break;
		}
	}
	return 0;
}
