import javax.persistence.*;

@Entity
@Table(name="COUNTRY")
public class Country {
    @Id
    @GeneratedValue
    @Column(name="CTRY_ID")
    private Integer id;

    @Column(name="CTRY_NAME")
    private String name;

    private int area;

    @Transient
    private int rank;

    // Annotate methods rather than fields to instruct Hibernate to
    // manipulate variables using their accessors.
    // FIXME: but setter how?
    //@Id
    //@GeneratedValue
    //@Column(name="CTRY_ID")
    public Integer getId() {
        return id;
    }

    public void setId(Integer value) {
        id = value;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setArea(int area) {
        this.area = area;
    }

    public int getArea() {
        return area;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public int getRank() {
        return rank;
    }

// Accessors

}