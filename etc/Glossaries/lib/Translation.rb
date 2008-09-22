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

