/*
  File autogenerated by gengetopt version 2.22.6
  generated with the following command:
  gengetopt -u -i tmpl.ggo 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "cmdline.h"

const char *gengetopt_args_info_purpose = "Generates mkstemp(3) file and writes output of PROGRAM to it.";

const char *gengetopt_args_info_usage = "Usage: tmpl [OPTIONS]... [FILES]...";

const char *gengetopt_args_info_versiontext = "This software is supplied WITHOUT ANY WARRANTY; without even the implied\nwarranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free\nsoftware, and you are welcome to redistribute it under certain conditions; see\nthe template COPYING for details.";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_detailed_help[] = {
  "  -h, --help                    Print help and exit",
  "      --detailed-help           Print help, including all details and hidden\n                                  options, and exit",
  "  -V, --version                 Print version and exit",
  "\n",
  "  -f, --force                   Force output generation even if PROGRAM fails\n                                  on a template. Use with caution!\n                                  (default=off)",
  "  If PROGRAM fails on a template, no data (from that template)\n  will be added to the global buffer, instead of aborting. This might lead to\n  unwanted\n  behaviour if you use tmpl for config file generation.\n",
  "  -T, --mkstemp-template=FORMAT Set mkstemp(3) template.\n                                  (default=`/tmp/.tmpl-XXXXXXXXXX')",
  "  See mkstemp(3) man page",
  "  -e, --environment=KEY=VALUE   Set environment variable ENV to VALUE prior to\n                                  running PROGRAM or COMMAND",
  "  -d, --delete=N                Spawns new process which deletes mkstemp(3)\n                                  file after N seconds.",
  "  -p, --program=PROGRAM         Pass templateN to PROGRAM.  (default=`/bin/sh')",
  "  Example: tmpl -p /usr/local/bin/ruby ~/.mutt.tmpl.rb\n",
  "",
  "  -c, --cat                     Print buffer to STDOUT (does not write\n                                  mkstemp(3) file)  (default=off)",
  "  -r, --run=COMMAND             Run COMMAND and delete template afterwards.",
  "  Instead of returning the path, tmpl runs COMMAND\n  and deletes the mkstemp(3) file after COMMAND returns.\n  It then exits with the return code of COMMAND.\n\n  Example: tmpl -r \"neomutt -F %f\" ~/.mutt.tmpl.sh\n\n  Variables:\n          %f  - The generated mkstemp(3) file path\n",
  "  -B, --background              Fork to background when used with -r\n                                  (default=off)",
  "      --stdout=FILE             Redirect STDOUT from -r to FILE",
  "      --stderr=FILE             Redirect STDERR from -r to FILE",
  "\nSee tmpl(1) for more informations and examples.",
    0
};

static void
init_help_array(void)
{
  gengetopt_args_info_help[0] = gengetopt_args_info_detailed_help[0];
  gengetopt_args_info_help[1] = gengetopt_args_info_detailed_help[1];
  gengetopt_args_info_help[2] = gengetopt_args_info_detailed_help[2];
  gengetopt_args_info_help[3] = gengetopt_args_info_detailed_help[3];
  gengetopt_args_info_help[4] = gengetopt_args_info_detailed_help[4];
  gengetopt_args_info_help[5] = gengetopt_args_info_detailed_help[6];
  gengetopt_args_info_help[6] = gengetopt_args_info_detailed_help[8];
  gengetopt_args_info_help[7] = gengetopt_args_info_detailed_help[9];
  gengetopt_args_info_help[8] = gengetopt_args_info_detailed_help[10];
  gengetopt_args_info_help[9] = gengetopt_args_info_detailed_help[12];
  gengetopt_args_info_help[10] = gengetopt_args_info_detailed_help[13];
  gengetopt_args_info_help[11] = gengetopt_args_info_detailed_help[14];
  gengetopt_args_info_help[12] = gengetopt_args_info_detailed_help[16];
  gengetopt_args_info_help[13] = gengetopt_args_info_detailed_help[17];
  gengetopt_args_info_help[14] = gengetopt_args_info_detailed_help[18];
  gengetopt_args_info_help[15] = gengetopt_args_info_detailed_help[19];
  gengetopt_args_info_help[16] = 0; 
  
}

const char *gengetopt_args_info_help[17];

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_FLOAT
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);

static int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->detailed_help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->force_given = 0 ;
  args_info->mkstemp_template_given = 0 ;
  args_info->environment_given = 0 ;
  args_info->delete_given = 0 ;
  args_info->program_given = 0 ;
  args_info->cat_given = 0 ;
  args_info->run_given = 0 ;
  args_info->background_given = 0 ;
  args_info->stdout_given = 0 ;
  args_info->stderr_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->force_flag = 0;
  args_info->mkstemp_template_arg = gengetopt_strdup ("/tmp/.tmpl-XXXXXXXXXX");
  args_info->mkstemp_template_orig = NULL;
  args_info->environment_arg = NULL;
  args_info->environment_orig = NULL;
  args_info->delete_orig = NULL;
  args_info->program_arg = gengetopt_strdup ("/bin/sh");
  args_info->program_orig = NULL;
  args_info->cat_flag = 0;
  args_info->run_arg = NULL;
  args_info->run_orig = NULL;
  args_info->background_flag = 0;
  args_info->stdout_arg = NULL;
  args_info->stdout_orig = NULL;
  args_info->stderr_arg = NULL;
  args_info->stderr_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{

  init_help_array(); 
  args_info->help_help = gengetopt_args_info_detailed_help[0] ;
  args_info->detailed_help_help = gengetopt_args_info_detailed_help[1] ;
  args_info->version_help = gengetopt_args_info_detailed_help[2] ;
  args_info->force_help = gengetopt_args_info_detailed_help[4] ;
  args_info->mkstemp_template_help = gengetopt_args_info_detailed_help[6] ;
  args_info->environment_help = gengetopt_args_info_detailed_help[8] ;
  args_info->environment_min = 0;
  args_info->environment_max = 0;
  args_info->delete_help = gengetopt_args_info_detailed_help[9] ;
  args_info->program_help = gengetopt_args_info_detailed_help[10] ;
  args_info->cat_help = gengetopt_args_info_detailed_help[13] ;
  args_info->run_help = gengetopt_args_info_detailed_help[14] ;
  args_info->background_help = gengetopt_args_info_detailed_help[16] ;
  args_info->stdout_help = gengetopt_args_info_detailed_help[17] ;
  args_info->stderr_help = gengetopt_args_info_detailed_help[18] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CMDLINE_PARSER_PACKAGE_NAME) ? CMDLINE_PARSER_PACKAGE_NAME : CMDLINE_PARSER_PACKAGE),
     CMDLINE_PARSER_VERSION);

  if (strlen(gengetopt_args_info_versiontext) > 0)
    printf("\n%s\n", gengetopt_args_info_versiontext);
}

static void print_help_common(void) {
  cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n\n", gengetopt_args_info_description);
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_print_detailed_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_detailed_help[i])
    printf("%s\n", gengetopt_args_info_detailed_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);

  args_info->inputs = 0;
  args_info->inputs_num = 0;
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}

/** @brief generic value variable */
union generic_value {
    float float_arg;
    char *string_arg;
    const char *default_string_arg;
};

/** @brief holds temporary values for multiple options */
struct generic_list
{
  union generic_value arg;
  char *orig;
  struct generic_list *next;
};

/**
 * @brief add a node at the head of the list 
 */
static void add_node(struct generic_list **list) {
  struct generic_list *new_node = (struct generic_list *) malloc (sizeof (struct generic_list));
  new_node->next = *list;
  *list = new_node;
  new_node->arg.string_arg = 0;
  new_node->orig = 0;
}


static void
free_multiple_string_field(unsigned int len, char ***arg, char ***orig)
{
  unsigned int i;
  if (*arg) {
    for (i = 0; i < len; ++i)
      {
        free_string_field(&((*arg)[i]));
        free_string_field(&((*orig)[i]));
      }
    free_string_field(&((*arg)[0])); /* free default string */

    free (*arg);
    *arg = 0;
    free (*orig);
    *orig = 0;
  }
}

static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{
  unsigned int i;
  free_string_field (&(args_info->mkstemp_template_arg));
  free_string_field (&(args_info->mkstemp_template_orig));
  free_multiple_string_field (args_info->environment_given, &(args_info->environment_arg), &(args_info->environment_orig));
  free_string_field (&(args_info->delete_orig));
  free_string_field (&(args_info->program_arg));
  free_string_field (&(args_info->program_orig));
  free_string_field (&(args_info->run_arg));
  free_string_field (&(args_info->run_orig));
  free_string_field (&(args_info->stdout_arg));
  free_string_field (&(args_info->stdout_orig));
  free_string_field (&(args_info->stderr_arg));
  free_string_field (&(args_info->stderr_orig));
  
  
  for (i = 0; i < args_info->inputs_num; ++i)
    free (args_info->inputs [i]);

  if (args_info->inputs_num)
    free (args_info->inputs);

  clear_given (args_info);
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  FIX_UNUSED (values);
  if (arg) {
    fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}

static void
write_multiple_into_file(FILE *outfile, int len, const char *opt, char **arg, const char *values[])
{
  int i;
  
  for (i = 0; i < len; ++i)
    write_into_file(outfile, opt, (arg ? arg[i] : 0), values);
}

int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->detailed_help_given)
    write_into_file(outfile, "detailed-help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->force_given)
    write_into_file(outfile, "force", 0, 0 );
  if (args_info->mkstemp_template_given)
    write_into_file(outfile, "mkstemp-template", args_info->mkstemp_template_orig, 0);
  write_multiple_into_file(outfile, args_info->environment_given, "environment", args_info->environment_orig, 0);
  if (args_info->delete_given)
    write_into_file(outfile, "delete", args_info->delete_orig, 0);
  if (args_info->program_given)
    write_into_file(outfile, "program", args_info->program_orig, 0);
  if (args_info->cat_given)
    write_into_file(outfile, "cat", 0, 0 );
  if (args_info->run_given)
    write_into_file(outfile, "run", args_info->run_orig, 0);
  if (args_info->background_given)
    write_into_file(outfile, "background", 0, 0 );
  if (args_info->stdout_given)
    write_into_file(outfile, "stdout", args_info->stdout_orig, 0);
  if (args_info->stderr_given)
    write_into_file(outfile, "stderr", args_info->stderr_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

static char *
get_multiple_arg_token(const char *arg)
{
  const char *tok;
  char *ret;
  size_t len, num_of_escape, i, j;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');
  num_of_escape = 0;

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
          ++num_of_escape;
        }
      else
        break;
    }

  if (tok)
    len = (size_t)(tok - arg + 1);
  else
    len = strlen (arg) + 1;

  len -= num_of_escape;

  ret = (char *) malloc (len);

  i = 0;
  j = 0;
  while (arg[i] && (j < len-1))
    {
      if (arg[i] == '\\' && 
	  arg[ i + 1 ] && 
	  arg[ i + 1 ] == ',')
        ++i;

      ret[j++] = arg[i++];
    }

  ret[len-1] = '\0';

  return ret;
}

static const char *
get_multiple_arg_token_next(const char *arg)
{
  const char *tok;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
        }
      else
        break;
    }

  if (! tok || strlen(tok) == 1)
    return 0;

  return tok+1;
}

static int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc);

int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc)
{
  int error_occurred = 0;

  if (option_given && (min > 0 || max > 0))
    {
      if (min > 0 && max > 0)
        {
          if (min == max)
            {
              /* specific occurrences */
              if (option_given != (unsigned int) min)
                {
                  fprintf (stderr, "%s: %s option occurrences must be %d\n",
                    prog_name, option_desc, min);
                  error_occurred = 1;
                }
            }
          else if (option_given < (unsigned int) min
                || option_given > (unsigned int) max)
            {
              /* range occurrences */
              fprintf (stderr, "%s: %s option occurrences must be between %d and %d\n",
                prog_name, option_desc, min, max);
              error_occurred = 1;
            }
        }
      else if (min > 0)
        {
          /* at least check */
          if (option_given < min)
            {
              fprintf (stderr, "%s: %s option occurrences must be at least %d\n",
                prog_name, option_desc, min);
              error_occurred = 1;
            }
        }
      else if (max > 0)
        {
          /* at most check */
          if (option_given > max)
            {
              fprintf (stderr, "%s: %s option occurrences must be at most %d\n",
                prog_name, option_desc, max);
              error_occurred = 1;
            }
        }
    }
    
  return error_occurred;
}
int
cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error_occurred = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  if (check_multiple_option_occurrences(prog_name, args_info->environment_given, args_info->environment_min, args_info->environment_max, "'--environment' ('-e')"))
     error_occurred = 1;
  
  
  /* checks for dependences among options */

  return error_occurred;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  FIX_UNUSED (default_value);
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_FLOAT:
    if (val) *((float *)field) = (float)strtod (val, &stop_char);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_FLOAT:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}

/**
 * @brief store information about a multiple option in a temporary list
 * @param list where to (temporarily) store multiple options
 */
static
int update_multiple_arg_temp(struct generic_list **list,
               unsigned int *prev_given, const char *val,
               const char *possible_values[], const char *default_value,
               cmdline_parser_arg_type arg_type,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  /* store single arguments */
  char *multi_token;
  const char *multi_next;

  if (arg_type == ARG_NO) {
    (*prev_given)++;
    return 0; /* OK */
  }

  multi_token = get_multiple_arg_token(val);
  multi_next = get_multiple_arg_token_next (val);

  while (1)
    {
      add_node (list);
      if (update_arg((void *)&((*list)->arg), &((*list)->orig), 0,
          prev_given, multi_token, possible_values, default_value, 
          arg_type, 0, 1, 1, 1, long_opt, short_opt, additional_error)) {
        if (multi_token) free(multi_token);
        return 1; /* failure */
      }

      if (multi_next)
        {
          multi_token = get_multiple_arg_token(multi_next);
          multi_next = get_multiple_arg_token_next (multi_next);
        }
      else
        break;
    }

  return 0; /* OK */
}

/**
 * @brief free the passed list (including possible string argument)
 */
static
void free_list(struct generic_list *list, short string_arg)
{
  if (list) {
    struct generic_list *tmp;
    while (list)
      {
        tmp = list;
        if (string_arg && list->arg.string_arg)
          free (list->arg.string_arg);
        if (list->orig)
          free (list->orig);
        list = list->next;
        free (tmp);
      }
  }
}

/**
 * @brief updates a multiple option starting from the passed list
 */
static
void update_multiple_arg(void *field, char ***orig_field,
               unsigned int field_given, unsigned int prev_given, union generic_value *default_value,
               cmdline_parser_arg_type arg_type,
               struct generic_list *list)
{
  int i;
  struct generic_list *tmp;

  if (prev_given && list) {
    *orig_field = (char **) realloc (*orig_field, (field_given + prev_given) * sizeof (char *));

    switch(arg_type) {
    case ARG_FLOAT:
      *((float **)field) = (float *)realloc (*((float **)field), (field_given + prev_given) * sizeof (float)); break;
    case ARG_STRING:
      *((char ***)field) = (char **)realloc (*((char ***)field), (field_given + prev_given) * sizeof (char *)); break;
    default:
      break;
    };
    
    for (i = (prev_given - 1); i >= 0; --i)
      {
        tmp = list;
        
        switch(arg_type) {
        case ARG_FLOAT:
          (*((float **)field))[i + field_given] = tmp->arg.float_arg; break;
        case ARG_STRING:
          (*((char ***)field))[i + field_given] = tmp->arg.string_arg; break;
        default:
          break;
        }        
        (*orig_field) [i + field_given] = list->orig;
        list = list->next;
        free (tmp);
      }
  } else { /* set the default value */
    if (default_value && ! field_given) {
      switch(arg_type) {
      case ARG_FLOAT:
        if (! *((float **)field)) {
          *((float **)field) = (float *)malloc (sizeof (float));
          (*((float **)field))[0] = default_value->float_arg;
        }
        break;
      case ARG_STRING:
        if (! *((char ***)field)) {
          *((char ***)field) = (char **)malloc (sizeof (char *));
          (*((char ***)field))[0] = gengetopt_strdup(default_value->string_arg);
        }
        break;
      default: break;
      }
      if (!(*orig_field)) {
        *orig_field = (char **) malloc (sizeof (char *));
        (*orig_field)[0] = 0;
      }
    }
  }
}

int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  struct generic_list * environment_list = NULL;
  int error_occurred = 0;
  struct gengetopt_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "detailed-help",	0, NULL, 0 },
        { "version",	0, NULL, 'V' },
        { "force",	0, NULL, 'f' },
        { "mkstemp-template",	1, NULL, 'T' },
        { "environment",	1, NULL, 'e' },
        { "delete",	1, NULL, 'd' },
        { "program",	1, NULL, 'p' },
        { "cat",	0, NULL, 'c' },
        { "run",	1, NULL, 'r' },
        { "background",	0, NULL, 'B' },
        { "stdout",	1, NULL, 0 },
        { "stderr",	1, NULL, 0 },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVfT:e:d:p:cr:B", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'f':	/* Force output generation even if PROGRAM fails on a template. Use with caution!.  */
        
        
          if (update_arg((void *)&(args_info->force_flag), 0, &(args_info->force_given),
              &(local_args_info.force_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "force", 'f',
              additional_error))
            goto failure;
        
          break;
        case 'T':	/* Set mkstemp(3) template..  */
        
        
          if (update_arg( (void *)&(args_info->mkstemp_template_arg), 
               &(args_info->mkstemp_template_orig), &(args_info->mkstemp_template_given),
              &(local_args_info.mkstemp_template_given), optarg, 0, "/tmp/.tmpl-XXXXXXXXXX", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "mkstemp-template", 'T',
              additional_error))
            goto failure;
        
          break;
        case 'e':	/* Set environment variable ENV to VALUE prior to running PROGRAM or COMMAND.  */
        
          if (update_multiple_arg_temp(&environment_list, 
              &(local_args_info.environment_given), optarg, 0, 0, ARG_STRING,
              "environment", 'e',
              additional_error))
            goto failure;
        
          break;
        case 'd':	/* Spawns new process which deletes mkstemp(3) file after N seconds..  */
        
        
          if (update_arg( (void *)&(args_info->delete_arg), 
               &(args_info->delete_orig), &(args_info->delete_given),
              &(local_args_info.delete_given), optarg, 0, 0, ARG_FLOAT,
              check_ambiguity, override, 0, 0,
              "delete", 'd',
              additional_error))
            goto failure;
        
          break;
        case 'p':	/* Pass templateN to PROGRAM..  */
        
        
          if (update_arg( (void *)&(args_info->program_arg), 
               &(args_info->program_orig), &(args_info->program_given),
              &(local_args_info.program_given), optarg, 0, "/bin/sh", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "program", 'p',
              additional_error))
            goto failure;
        
          break;
        case 'c':	/* Print buffer to STDOUT (does not write mkstemp(3) file).  */
        
        
          if (update_arg((void *)&(args_info->cat_flag), 0, &(args_info->cat_given),
              &(local_args_info.cat_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "cat", 'c',
              additional_error))
            goto failure;
        
          break;
        case 'r':	/* Run COMMAND and delete template afterwards..  */
        
        
          if (update_arg( (void *)&(args_info->run_arg), 
               &(args_info->run_orig), &(args_info->run_given),
              &(local_args_info.run_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "run", 'r',
              additional_error))
            goto failure;
        
          break;
        case 'B':	/* Fork to background when used with -r.  */
        
        
          if (update_arg((void *)&(args_info->background_flag), 0, &(args_info->background_given),
              &(local_args_info.background_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "background", 'B',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          if (strcmp (long_options[option_index].name, "detailed-help") == 0) {
            cmdline_parser_print_detailed_help ();
            cmdline_parser_free (&local_args_info);
            exit (EXIT_SUCCESS);
          }

          /* Redirect STDOUT from -r to FILE.  */
          if (strcmp (long_options[option_index].name, "stdout") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->stdout_arg), 
                 &(args_info->stdout_orig), &(args_info->stdout_given),
                &(local_args_info.stdout_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "stdout", '-',
                additional_error))
              goto failure;
          
          }
          /* Redirect STDERR from -r to FILE.  */
          else if (strcmp (long_options[option_index].name, "stderr") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->stderr_arg), 
                 &(args_info->stderr_orig), &(args_info->stderr_given),
                &(local_args_info.stderr_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "stderr", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */


  update_multiple_arg((void *)&(args_info->environment_arg),
    &(args_info->environment_orig), args_info->environment_given,
    local_args_info.environment_given, 0,
    ARG_STRING, environment_list);

  args_info->environment_given += local_args_info.environment_given;
  local_args_info.environment_given = 0;
  
  if (check_required)
    {
      error_occurred += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  if (optind < argc)
    {
      int i = 0 ;
      int found_prog_name = 0;
      /* whether program name, i.e., argv[0], is in the remaining args
         (this may happen with some implementations of getopt,
          but surely not with the one included by gengetopt) */

      i = optind;
      while (i < argc)
        if (argv[i++] == argv[0]) {
          found_prog_name = 1;
          break;
        }
      i = 0;

      args_info->inputs_num = argc - optind - found_prog_name;
      args_info->inputs =
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        if (argv[optind++] != argv[0])
          args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind-1]) ;
    }

  return 0;

failure:
  free_list (environment_list, 1 );
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
