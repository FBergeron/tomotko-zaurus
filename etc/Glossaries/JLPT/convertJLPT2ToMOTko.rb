#!/usr/bin/ruby -KU

# The glossary have to be renamed by hand after the conversion.

glossaryName = 'New glossary'

termId = 1

pattern1 = %r{(.*)\s\[(.*)\]\s/(.*)/}
pattern2 = %r{(.*)\s/(.*)/}

puts "<?xml version=\"1.0\"?>"
puts "<!-- Copyright (C) 2001 Thierry Bézecourt (http://www.thbz.org) -->"
puts "<glossary id=\"0\" name=\"" + glossaryName + "\">"

while( line = gets ) 
    line.chomp!

    if( line =~ pattern1 ) 
        kanji, furigana, english = $1, $2, $3
        english.gsub!( %r{/}, ',' )
    elsif( line =~ pattern2 )
        kanji, furigana, english = $1, $1, $2
        english.gsub!( %r{/}, ',' )
    end
    comment = nil

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

        termId += 1
    end

end

puts "</glossary>"
