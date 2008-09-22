#!/usr/bin/ruby -KU

require "../lib/Glossary.rb"
require "../lib/Term.rb"
require "../lib/Translation.rb"

glossary = nil
term = nil
comment = nil
kanji = furigana = english = nil
ex_kanji = ex_furigana = ex_english = nil

while( line = gets ) 
    line.chomp!

    if( line =~ /^(\d+)$/ )
        lesson = $1
        
        if( lesson.to_i < 10 )
            lesson = "0#{lesson}"
        end

        term.addComment( 'en', 'ja', comment ) if( term != nil && comment != nil )
        puts glossary.writeXmlFile if( glossary )

        glossary = Glossary.new( lesson )
    elsif( line =~ /^\t(.*)/ )
        example = $1
        ex_kanji, ex_furigana, ex_english = example.split( /\t/ )
        formattedExample = "#{ex_kanji}; #{ex_furigana}; #{ex_english}\n"
        if( comment == nil )
            comment = formattedExample 
        else
            comment = comment + formattedExample 
        end
    elsif( line =~ /^http/ || line =~ /^$/ )
        # Ignore empty lines or those beginning with an url.
    else
        term.addComment( 'en', 'ja', comment ) if( term != nil && comment != nil )

        kanji, furigana, english = line.split( /\t/ )
        comment = nil
        trans_ja = Translation.new( 'ja', kanji, furigana )
        trans_en = Translation.new( 'en', english )
        term = Term.new
        term.addTranslation( trans_ja )
        term.addTranslation( trans_en )
        glossary.addTerm( term )
    end

end

term.addComment( 'en', 'ja', comment ) if( term != nil && comment != nil )
puts glossary.writeXmlFile if( glossary )

exit
