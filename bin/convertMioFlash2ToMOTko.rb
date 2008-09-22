#!/usr/bin/ruby -KU

glossaryName = 'New glossary'

termId = 1

puts "<?xml version=\"1.0\"?>"
puts "<glossary id=\"0\" name=\"" + glossaryName + "\">"

while( line = gets ) 
    line.chomp!
    kanji, furigana, english, comment = line.split( %r{\s*/\s*} )

    if( kanji || furigana || english )
        puts "  <term id=\"" + termId.to_s + "\">"
        if( english )
            puts "    <trans lang=\"en\">"
            puts "      <word>" + english + "</word>"
            puts "    </trans>"
        end
        if( kanji || furigana )
            puts "    <trans lang=\"ja\">"
            puts "      <word>" + kanji + "</word>" if( kanji )
            puts "      <alt>" + furigana + "</alt>" if( furigana )
            puts "      <comment>" + comment + "</comment>" if( comment )
        end
        puts "    </trans>"
        puts "  </term>"
    end

    termId += 1
end

puts "</glossary>"
