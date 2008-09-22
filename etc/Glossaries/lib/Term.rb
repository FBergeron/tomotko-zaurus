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

