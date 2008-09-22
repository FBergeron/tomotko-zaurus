#!/usr/bin/ruby -KU

class Glossary

    attr_reader :title, :terms
    attr_writer :title

    def initialize( title = nil )
        @termId = 0
        @title = title
        @terms = Hash.new
    end

    def addTerm( term )
        @termId += 1
        term.id = @termId
        @terms[ @termId ] = term
    end

    def to_s
        @title + "={" + @terms.to_s + "}"
    end

    def to_xml
        str = "<glossary name=\"#{@title}\">\n"
        @terms.keys().sort().each {
            | key |
            str += @terms[ key ].to_xml
        }
        str += "</glossary>\n"
    end

end

class Term
   
    attr_reader :id, :translations, :comments
    attr_writer :id

    def initialize( id = 0 )
        @id = id
        @translations = Hash.new
        @comments = Hash.new
    end

    def addTranslation( trans )
        @translations[ trans.language ] = trans
    end

    def addComment( lang1, lang2, comment )
        @comments[ "#{lang1}-#{lang2}" ] = comment
    end

    def to_s
        "#{@id}=[#{@translations}]"
    end
    
    def to_xml
        str = "\t<term id=\"#{@id}\">\n"
        @translations.keys().sort().each {
            | key |
            str += @translations[ key ].to_xml
        }
        @comments.keys().sort().each {
            | key |
            lang1, lang2 = key.split( /-/ )
            str += "\t\t<comment languages=\"#{lang1}-#{lang2}\">#{@comments[ key ]}</comment>\n"
        }
        str += "\t</term>\n"
        str
    end

end

class Translation

    attr_reader :language, :word, :alt

    def initialize( language, word, alt = nil, comment = nil )
        @language = language
        @word = word
        @alt = alt
    end

    def to_s
        "#{@language}:#{@word},#{@alt},#{@comment}"
    end

    def to_xml
        str = "\t\t<trans lang=\"#{@language}\">\n"
        str += "\t\t\t<word>#{@word}</word>\n" if @word
        str += "\t\t\t<alt>#{@alt}</alt>\n" if @alt
        str += "\t\t</trans>\n"
        str
    end

end


$kurochka = false

glossaries = Hash.new
spanishInputFile = nil
lesson = nil
File.open( "English2.txt" ) {
    | englishInputFile |
    englishInputFile.each_line {
        | englishLine |
        if( englishLine =~ %r{^(\d\d)$} )
            spanishInputFile.close() if spanishInputFile
            lesson = englishLine.to_i
            number = ( lesson < 10 ? "0#{lesson}" : "#{lesson}" )
            glossaries[ lesson ] = Glossary.new
            spanishInputFile = File.new( "NihongoShoho#{number}.txt" )
        elsif
            spanishLine = spanishInputFile.readline()
            if( spanishLine =~ %r{(.*)/(.*)/(.*)} )
                kanji = $1
                alt = $2
                romaji = $3

                if( $kurochka )
                    trans_ja = Translation.new( 'ja', kanji, alt )
                    trans_en = Translation.new( 'en', englishLine.chomp )
                    term = Term.new
                    term.addTranslation( trans_ja )
                    term.addTranslation( trans_en )
                    term.addComment( 'en', 'ja', romaji )
                    glossaries[ lesson ].addTerm( term )
                else
                    trans_ja = Translation.new( 'ja', kanji, alt )
                    trans_es = Translation.new( 'es', romaji )
                    trans_en = Translation.new( 'en', englishLine.chomp )
                    term = Term.new
                    term.addTranslation( trans_ja )
                    term.addTranslation( trans_es )
                    term.addTranslation( trans_en )
                    glossaries[ lesson ].addTerm( term )
                end
            end
        end
    }
}

glossaries.keys().sort().each {
    | key |
    number = ( key < 10 ? "0#{key}" : "#{key}" )
    File.open( "NihongoShoho#{number}.xml", "w" ) {
        | glossaryFile |
        glossaryFile.puts "<?xml version=\"1.0\"?>"
        glossaryFile.puts glossaries[ key ].to_xml
    }
}

exit
#
#for i in 6 .. 34
#    number = ( i < 10 ? "0#{i}" : "#{i}" )
#    glossaries[ i ] = Glossary.new
#    File.open( "NihongoShoho#{number}.txt" ) {
#        | inputFile |
#
#        inputFile.each_line {
#            | line |
#            kanji = alt = romaji = nil
#            if( line =~ %r{(.*)/(.*)/(.*)} )
#                kanji = $1
#                alt = $2
#                romaji = $3
#
#                trans_ja = Translation.new( 'ja', kanji, alt )
#                trans_es = Translation.new( 'es', romaji )
#                term = Term.new
#                term.addTranslation( trans_ja )
#                term.addTranslation( trans_es )
#                glossary[ i ].addTerm( term )
#            end
#        }
#    }
#    #puts glossary.to_xml()
#end
