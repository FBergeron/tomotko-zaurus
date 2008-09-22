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

    def writeXmlFile 
        File.open( "#{@title}.xml", "w+" ) {
            | file |
            file.puts( "<?xml version=\"1.0\"?>" )
            file.puts( to_xml )
        }
    end

end

