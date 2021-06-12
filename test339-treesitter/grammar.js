module.exports = grammar({
  name: "MyCommandLanguage",

  rules: {
    source_file: $ => repeat($.command),

    command: $ => seq(
      $.identifier,
      $.argument_list
    ),

    argument_list: $ => seq(
      repeat($.expression),
      ";"
    ),

    expression: $ => choice(
      $.string,
      $.number
    ),

    identifier: $ => /[a-z]+/,

    string: $ => seq(
      '"',
      /[^"]+/,
      '"'
    ),

    number: $ => /\d+/,
  }
});
